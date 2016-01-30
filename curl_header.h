#ifndef INCLUDE_CURL_HEADER_H
#define INCLUDE_CURL_HEADER_H

enum HandleType
{
    Handle_Invalid = -1,
    Handle_Curl = 0,
    Handle_CurlFile,
    Handle_CurlHeader
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
