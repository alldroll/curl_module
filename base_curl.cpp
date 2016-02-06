#include <stdlib.h>
#include "curl_header.h"
#include "curl_wrap.h"


static void FreeCurl(void* p, unsigned int num) {
    Curl* curl = (Curl*) p;
    delete curl;
}

// native Handle:curl_init();
static cell AMX_NATIVE_CALL AMX_CurlInit(AMX* amx, cell* params) {
    Curl* curl = Curl::Initialize();
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Couldn't alloc curl handle");
        return -1;
    }

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

// native CURLcode:curl_setopt_cell(Handle:h, CURLoption:opt, value)
static cell AMX_NATIVE_CALL AMX_CurlSetOptCell(AMX* amx, cell* params) {
    Curl* curl = (Curl*)GetHandle(params[1], HANDLE_CURL);
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        return -1;
    }

    CURLoption opt = (CURLoption)params[2];
    if (!curl_module_is_cell_option(opt)) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Unsupported cell option: %d", opt);
        return -1;
    }

    return curl->SetOptionInteger(opt, (int)params[3]);
}

// native CURLcode:curl_setopt_string(Handle:h, CURLoption:opt, const val[])
static cell AMX_NATIVE_CALL AMX_CurlSetOptString(AMX* amx, cell* params) {
    Curl* curl = (Curl*)GetHandle(params[1], HANDLE_CURL);
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        return -1;
    }

    CURLoption opt = (CURLoption)params[2];
    if (!curl_module_is_string_option(opt)) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Unsupported string option: %d", opt);
        return -1;
    }

    int len;
    char* val = MF_GetAmxString(amx, params[1], 0, &len);
    return curl->SetOptionString(opt, val);
}

AMX_NATIVE_INFO g_BaseCurlNatives[] = {
    {"curl_init", AMX_CurlInit},
    {"curl_close", AMX_CurlClose},
    {"curl_reset", AMX_CurlReset},
    {"curl_errno", AMX_CurlErrno},
    {"curl_setopt_cell", AMX_CurlSetOptCell},
    {"curl_setopt_string", AMX_CurlSetOptString},
    {NULL,                NULL},
};
