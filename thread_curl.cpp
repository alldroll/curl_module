#include "thread_curl.h"
#include <sh_stack.h>

/*
 * inspired by amxmodx mysqlx module
 */

using namespace SourceMod;

static MainThreader g_Threader;
static ThreadWorker* g_Worker;
static CStack<CurlThread*> g_ThreadQueue;
static IMutex* g_QueueLock;

CurlThread::CurlThread():
    curl_(NULL), forward_(0),
    handle_(-1), data_(0) {
}

CurlThread::~CurlThread() {
    Clean();
}

void CurlThread::Clean() {
    if (forward_) {
        MF_UnregisterSPForward(forward_);
    }

    delete curl_;
    forward_ = 0;
    data_ = 0;
    handle_ = -1;
}

void CurlThread::SetCurl(Curl* curl) {
    curl_ = curl;
}

void CurlThread::SetForward(cell forward) {
    forward_ = forward;
}

void CurlThread::SetHandle(cell handle) {
    handle_ = handle;
}

void CurlThread::SetCellData(cell data) {
    data_ = data;
}

void CurlThread::Execute() {
    CurlWrite& write_data = curl_->write_data();

    MF_ExecuteForward(
        forward_,
        handle_,
        curl_->last_error(),
        write_data.buffer.buf,
        data_ /* tmp */
    );
}

void CurlThread::RunThread(IThreadHandle* pHandle) {
    curl_->Exec();
}

void CurlThread::OnTerminate(IThreadHandle* pHandle, bool cancel) {
    if (!cancel) {
        g_QueueLock->Lock();
        g_ThreadQueue.push(this);
        g_QueueLock->Unlock();
    } else {
        delete curl_;
    }
}

void StartFrame() {
    if (g_Worker && g_QueueLock) {
        g_QueueLock->Lock();

        CurlThread* thread;
        while (!g_ThreadQueue.empty()) {
            thread = g_ThreadQueue.front();
            g_ThreadQueue.pop();
            g_QueueLock->Unlock();
            thread->Execute();
            delete thread;
            g_QueueLock->Lock();
        }

        g_QueueLock->Unlock();
    }

    RETURN_META(MRES_IGNORED);
}

void ShutdownThreading() {
    if (g_Worker) {
        g_Worker->SetMaxThreadsPerFrame(8192);
        g_Worker->Stop(true);
    }

    g_QueueLock->Lock();

    CurlThread* thread;
    while (!g_ThreadQueue.empty()) {
        thread = g_ThreadQueue.front();
        g_ThreadQueue.pop();
        g_QueueLock->Unlock();
        delete thread;
        g_QueueLock->Lock();
    }

    g_QueueLock->Unlock();
}

void OnPluginsLoaded() {
    if (g_Worker) {
        return;
    }

    g_Worker = new ThreadWorker(&g_Threader, DEFAULT_THINK_TIME_MS);
    if (!g_QueueLock) {
        g_QueueLock = g_Threader.MakeMutex();
    }

    if (!g_Worker->Start()) {
        delete g_Worker;
        g_Worker = NULL;
    }
}

void OnPluginsUnloading() {
    if (!g_Worker) {
        return;
    }

    g_Worker->SetMaxThreadsPerFrame(8192);
    g_Worker->Stop(false);
    delete g_Worker;
    g_Worker = NULL;

    g_QueueLock->Lock();

    CurlThread* thread;
    while (!g_ThreadQueue.empty()) {
        thread = g_ThreadQueue.front();
        g_ThreadQueue.pop();
        g_QueueLock->Unlock();
        delete thread;
        g_QueueLock->Lock();
    }

    g_QueueLock->Unlock();
}

/*
 * public OnComplete(Handle:curl, CURLcode:code, const response[], any:data)
 * native CURLcode:curl_thread_exec(Handle:h, const callback[], any:data)
 */
static cell AMX_NATIVE_CALL AMX_CurlThreadExec(AMX* amx, cell* params) {
    Curl* curl = (Curl*)GetHandle(params[1], HANDLE_CURL);
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        return -1;
    }

    Curl* duplicate = curl->MakeDuplicate();
    if (!duplicate) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Couldn't allocate duplicate");
        return -1;
    }

    int len;
    const char* callback = MF_GetAmxString(amx, params[2], 0, &len);
    int fwd = MF_RegisterSPForwardByName(
        amx, callback, FP_CELL, FP_CELL, FP_STRING, FP_CELL, FP_DONE
    );
    if (fwd < 1) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Function not found: %s", callback);
        return -1;
    }

    CurlThread* thread = new CurlThread();
    thread->SetCurl(duplicate);
    thread->SetForward(fwd);
    thread->SetCellData(params[3]);
    thread->SetHandle(params[1]);

    g_Worker->MakeThread(thread);

    return 1;
}

AMX_NATIVE_INFO g_ThreadCurlNatives[] = {
    {"curl_thread_exec", AMX_CurlThreadExec},
    {NULL, NULL}
};
