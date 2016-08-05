#ifndef INCLUDE_CURL_HEADER_H
#define INCLUDE_CURL_HEADER_H

#include "amxxmodule.h"
#include "curl_wrap.h"
#include "ThreadSupport.h"
#include "ThreadWorker.h"

enum HandleType
{
    HANDLE_INVALID = -1,
    HANDLE_CURL = 0,
    HANDLE_CURL_READ,
    HANDLE_CURL_WRITE,
    HANDLE_CURL_SLIST,
    HANDLE_CURL_FORM
};

typedef void (*FREEHANDLE)(void *, unsigned int);

extern unsigned int MakeHandle(void *ptr, HandleType type, FREEHANDLE f);
extern void *GetHandle(unsigned int num, HandleType type);
extern bool FreeHandle(unsigned int num);
extern void FreeAllHandles(HandleType type);
extern void FreeHandleTable();
extern void ShutdownThreading();

extern AMX_NATIVE_INFO g_BaseCurlNatives[];
extern AMX_NATIVE_INFO g_ThreadCurlNatives[];

#endif
