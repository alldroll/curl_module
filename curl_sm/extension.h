#ifndef CURL_EXT_H
#define CURL_EXT_H

#include "smsdk_ext.h"
#include "utils/threadpool.h"
#include "utils/curlapi.h"

/**
 *	
 */

class Curl_Ext : public SDKExtension
{
 public:
	virtual void SDK_OnUnload();
	virtual bool SDK_OnLoad(char* error, size_t err_max, bool late);
};

/**
 *	
 */

class CurlTypeHandler : public IHandleTypeDispatch
{
 public:
  virtual void OnHandleDestroy(HandleType_t type, void* object) {  
    if (object) { 
      delete reinterpret_cast<Curl*>(object);
    }
  }
};

extern ThreadPool* g_thread_pool; //
extern HandleType_t g_curl_type; //
extern HandleType_t g_file_type; //
extern const sp_nativeinfo_t kExtensionNatives[]; //



#endif // CURL_EXT_H

