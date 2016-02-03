#include "curl_header.h"
#include "curl_wrap.h"

static void FreeCurl(void* p, unsigned int num) {
    Curl* curl = (Curl*) p;
    delete curl;
}

// native Handle:curl_init();
static cell AMX_NATIVE_CALL AMX_CurlInit(AMX* amx, cell* params) {
    Curl* curl = Curl::Initialize();
    return MakeHandle(curl, HANDLE_CURL, FreeCurl);
}

// native curl_close(Handle:h);
static cell AMX_NATIVE_CALL AMX_CurlClose(AMX* amx, cell* params) {
    bool success = true;
    if (!FreeHandle(params[1])) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        success = false;
    }

    return success;
}

// native curl_reset(Handle:h);
static cell AMX_NATIVE_CALL AMX_CurlReset(AMX* amx, cell* params) {
    bool success = true;
    Curl* curl = (Curl*)GetHandle(params[1], HANDLE_CURL);
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        success = false;
    } else {
        curl->Reset();
    }

    return success;
}


// native CURLcode:curl_errno(Handle:h);
static cell AMX_NATIVE_CALL AMX_CurlErrno(AMX* amx, cell* params) {
    Curl* curl = (Curl*)GetHandle(params[1], HANDLE_CURL);
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        return -1;
    }

    return curl->last_error();
}


AMX_NATIVE_INFO g_BaseCurlNatives[] = {
    {"curl_init", AMX_CurlInit},
    {"curl_close", AMX_CurlClose},
    {"curl_reset", AMX_CurlReset},
    {"curl_errno", AMX_CurlErrno},
    {NULL,                NULL},
};