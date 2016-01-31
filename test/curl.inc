#if defined _curl_included
    #endinput
#endif
#define _curl_included

#pragma reqlib curl
#if !defined AMXMODX_NOAUTOLOAD
    #pragma loadlib curl
#endif

enum Handle
{
   INVALID = INVALID_HANDLE,
};

native Handle:curl_init();

native curl_close(Handle:h);



