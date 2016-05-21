#ifndef CURL_MODULE_THREAD_H
#define CURL_MODULE_THREAD_H

#include "IThreader.h"
#include "curl_header.h"
#include "curl_wrap.h"

class CurlThread : public IThread {
public:
    CurlThread();
    ~CurlThread();

    void SetCurl(Curl* curl);
    void SetForward(cell forward);
    void SetCellData(cell data);
    void SetHandle(cell handle);
    void Execute();

    void RunThread(IThreadHandle* pHandle);
    void OnTerminate(IThreadHandle* pHandle, bool cancel);

private:
    void Clean();

private:

    Curl* curl_;
    int forward_;
    cell handle_;
    cell data_;
};

#endif
