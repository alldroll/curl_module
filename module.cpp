#include "amxxmodule.h"
#include "utils/threadpool.h"
#include <curl/curl.h>

#define THREAD_NUM 2

static ThreadPool* g_ThreadPool = NULL;

// native curl_easy_init();
static cell AMX_NATIVE_CALL AMX_CurlEasyInit(AMX* amx, cell* params)
{
    return 1;
}

AMX_NATIVE_INFO NATIVES[] = 
{
    {"curl_easy_init", AMX_CurlEasyInit},
    {NULL,                         NULL}
};


void OnAmxxAttach()
{
    MF_AddNatives(NATIVES);

    //   long flags = CURL_GLOBAL_NOTHING;

    // #ifdef _WIN32
    //   flags = CURL_GLOBAL_WIN32;
    // #endif

    //   CURLcode code = curl_global_init(flags);

    //   if (code)
    //   {
    //     // smutils->Format(error, err_max, "%s", curl_easy_strerror(code));
    //     // return false;
    //   }

    g_ThreadPool = new ThreadPool(THREAD_NUM);
}

void OnAmxxDetach()
{
    delete g_ThreadPool;
    // curl_global_cleanup();
}

void OnPluginsUnloaded()
{
    //delete handle_table
}

// extern "C" void __cxa_pure_virtual(void)
// {
// }
