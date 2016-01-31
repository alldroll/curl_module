#include "curl_header.h"
#include "curl_wrap.h"

static void FreeCurl(void* p, unsigned int num) {
    Curl* curl = (Curl*) p;
    delete curl;
}

// native curl_init();
static cell AMX_NATIVE_CALL AMX_CurlInit(AMX* amx, cell* params) {
    Curl* curl = Curl::Initialize();
    return MakeHandle(curl, HANDLE_CURL, FreeCurl);
}

// native curl_close();
static cell AMX_NATIVE_CALL AMX_CurlClose(AMX* amx, cell* params) {
    int success = 1;
    if (!FreeHandle(params[1])) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        success = 0;
    }

    return success;
}

AMX_NATIVE_INFO g_BaseCurlNatives[] = {
    {"curl_init", AMX_CurlInit},
    {"curl_close", AMX_CurlClose},
    {NULL,                NULL},
};
