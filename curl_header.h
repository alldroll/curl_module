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
    HANDLE_CURL_HEADER
};

typedef void (*FREEHANDLE)(void *, unsigned int);

unsigned int MakeHandle(void *ptr, HandleType type, FREEHANDLE f);
void *GetHandle(unsigned int num, HandleType type);
bool FreeHandle(unsigned int num);
void FreeAllHandles(HandleType type);
void FreeHandleTable();

extern AMX_NATIVE_INFO g_BaseCurlNatives[];
extern AMX_NATIVE_INFO g_ThreadCurlNatives[];

#endif
