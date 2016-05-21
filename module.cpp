#include "amxxmodule.h"
#include "curl_header.h"

void OnAmxxAttach() {
    MF_AddNatives(g_BaseCurlNatives);
    MF_AddNatives(g_ThreadCurlNatives);

    long flags = CURL_GLOBAL_NOTHING;
#ifdef _WIN32
    flags = CURL_GLOBAL_WIN32;
#endif
    CURLcode code = curl_global_init(flags);
    if (code) {
        /* TODO report error */
    }
}

void OnAmxxDetach() {
    curl_global_cleanup();
    FreeHandleTable();
}

void OnPluginsUnloaded() {
    //FreeAllHandles(HANDLE_CURL); [> TODO maybe not <]
}

extern "C" void __cxa_pure_virtual(void) {
}
