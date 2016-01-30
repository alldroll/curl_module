#include "amxxmodule.h"

// native curl_easy_init();
static cell AMX_NATIVE_CALL AMX_CurlEasyInit(AMX* amx, cell* params)
{
    return 1;
}

AMX_NATIVE_INFO NATIVES[] =
{
    {"curl_init", AMX_CurlInit},
    {NULL,                         NULL}
};


void OnAmxxAttach()
{
    MF_AddNatives(NATIVES);

    long flags = CURL_GLOBAL_NOTHING;

#ifdef _WIN32
    flags = CURL_GLOBAL_WIN32;
#endif

    CURLcode code = curl_global_init(flags);

    if (code) {
        /* TODO report error */
    }
}

void OnAmxxDetach()
{
    curl_global_cleanup();
}

void OnPluginsUnloaded()
{
}
