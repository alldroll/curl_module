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
    void SetForward(int forward);
    void SetCellData(cell data);
    void Execute();

    void RunThread(IThreadHandle* pHandle);
    void OnTerminate(IThreadHandle* pHandle, bool cancel);

private:
    void Clean();

private:

    Curl* curl_;
    int forward_;
};

#endif
