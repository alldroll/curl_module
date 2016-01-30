#include "curl_header.h"
#include "curl_wrap.h"

// native curl_init();
static cell AMX_NATIVE_CALL AMX_CurlInit(AMX* amx, cell* params) {
    return 1;
}

// native curl_clean();
static cell AMX_NATIVE_CALL AMX_CurlClean(AMX* amx, cell* params) {
    return 1;
}

AMX_NATIVE_INFO g_BaseCurlNatives[] = {
    {"curl_init", AMX_CurlInit},
    {NULL,                NULL},
};


