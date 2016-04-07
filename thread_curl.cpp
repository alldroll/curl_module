#include "thread_curl.h"

using namespace SourceMod;

CurlThread::CurlThread(): curl_(NULL), forward_(0) {}

CurlThread::~CurlThread() {
    Clean();
}

void CurlThread::Clean() {
    /* TODO implement */
}

void CurlThread::SetCurl(Curl* curl) {
    curl_ = curl;
}

void CurlThread::SetForward(int forward) {
    forward_ = forward;
}

void CurlThread::Execute() {
    /* TODO implement */
}

void CurlThread::RunThread(IThreadHandle* pHandle) {
    /* TODO implement */
}

void CurlThread::OnTerminate(IThreadHandle* pHandle, bool cancel) {
    /* TODO implement */
}

/*
 * public OnComplete(Handle:curl, CURLcode:code, const response[], any:data)
 * native CURLcode:curl_thread_exec(Handle:h, const callback[], any:data)
 */
static cell AMX_NATIVE_CALL AMX_CurlThreadExec(AMX* amx, cell* params) {
    return 1;
}

AMX_NATIVE_INFO g_ThreadCurlNatives[] = {
    {"curl_thread_exec", AMX_CurlThreadExec},
    {NULL, NULL}
};
