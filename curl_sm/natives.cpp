#include "natives.h"

static void ExecuteCompleteCallback_Frame(void* pack);

/*
 * TODO what is this structure
 */

struct CompletePack
{
    IPluginFunction* pfunc;
    cell_t user_data;
    Handle_t handle;
    CURLcode error;
};

/*
 * TODO what is this class
 */

class CurlThreadExecuter : public IWork
{
    public:
        CurlThreadExecuter(Curl* curl, CompletePack* pack)
            curl_(curl), 
            pack_(pack) {  
            }

        virtual void Execute() {  
            if (curl_ == NULL) {	
                //TODO report problem
                return;
            }

            if (!curl_->Exec()) {
                pack_->error = curl_->last_error();
            }

            /**
             * Sourcemod api is not threadsafe, so we have to call 
             * complete callback on next available game frame
             */

            smutils->AddFrameAction(ExecuteCompleteCallback_Frame, pack_);

            /**
             * We have to delete curl handle
             * because it was duplicated for thread performing
             * see reason of duplucating below (CurlThreadExec_SM function)
             */ 

            delete curl_;	
        }

    private:
        Curl* curl_;
        CompletePack* pack_;
};

/**
 * Execute curl_exec complete callback
 */

inline void ExecuteCompleteCallback(
        IPluginFunction* pfunc,	
        Handle_t handle,	
        CURLcode error,	
        cell_t user_data) {	
    pfunc->PushCell(handle);
    pfunc->PushCell(error);
    pfunc->PushCell(user_data);
    pfunc->Execute(NULL);
}

/**
 * Execute curl_exec callback on next available game frame
 */
void ExecuteCompleteCallback_Frame(void* ptr) {	
    CompletePack* pack = static_cast<CompletePack*>(ptr);

    if (pack == NULL) {  
        //TODO report problem

        return;
    }

    IPluginFunction* pfunc = pack->pfunc;

    if (pfunc == NULL) {	
        //smutils->	report error
        return;
    }

    ExecuteCompleteCallback(pfunc, pack->handle, pack->error, pack->user_data);

    delete pack;
}

//native Handle:curl_copy_handle(Handle:curl);
static cell_t CurlCopyHandle_SM(IPluginContext* pcontext, const cell_t* params) {  
    Handle_t handle = static_cast<Handle_t>(params[1]);
    HandleError herr = HandleError_None;
    HandleSecurity sec(pcontext->GetIdentity(), myself->GetIdentity());;


    Curl* curl = NULL;

    if ((herr = handlesys->ReadHandle(handle, g_curl_type, &sec, (void**)curl))
            != HandleError_None) {  
        return pcontext->ThrowNativeError("Invalid curl handle %x (error %d)",
                params[1], herr);
    }
    Curl* duplicate = curl->MakeDuplicate();

    if (duplicate == NULL) {	
        return pcontext->ThrowNativeError("Invalid curl handle %x (error %d)",
                params[1], herr);
    }

    Handle_t duphandle = handlesys->CreateHandle(
            g_curl_type, 
            &duplicate, 
            pcontext->GetIdentity(), 
            myself->GetIdentity(),  
            NULL);

    return duphandle;
}

//native CURLcode:curl_errno(Handle:curl);
static cell_t CurlErrNo_SM(IPluginContext* pcontext, const cell_t* params) {  
    Handle_t handle = static_cast<Handle_t>(params[1]);
    HandleError herr = HandleError_None;
    HandleSecurity sec(pcontext->GetIdentity(), myself->GetIdentity());

    Curl* curl = NULL;

    if ((herr = handlesys->ReadHandle(handle, g_curl_type, &sec, (void**)curl))
            != HandleError_None) {  
        return pcontext->ThrowNativeError("Invalid curl handle %x (error %d)",
                params[1], herr);
    }

    return curl->last_error();
}

//native CURLcode:curl_error(Handle:curl, String:buffer[], buflen);
static cell_t CurlError_SM(IPluginContext* pcontext, const cell_t* params) {  
    Handle_t handle = static_cast<Handle_t>(params[1]);
    HandleError herr = HandleError_None;
    HandleSecurity sec(pcontext->GetIdentity(), myself->GetIdentity());

    Curl* curl = NULL;

    if ((herr = handlesys->ReadHandle(handle, g_curl_type, &sec, (void**)curl))
            != HandleError_None) {  
        return pcontext->ThrowNativeError("Invalid curl handle %x (error %d)",
                params[1], herr);
    }

    pcontext->StringToLocal(params[2], params[3], curl->StringifyLastError());

    return 1;
}

//native curl_escape(Handle:curl, const String:url[], String:buffer[], buflen);
static cell_t CurlEscape_SM(IPluginContext* pcontext, const cell_t* params) {  
    Handle_t handle = static_cast<Handle_t>(params[1]);
    HandleError herr = HandleError_None;
    HandleSecurity sec(pcontext->GetIdentity(), myself->GetIdentity());

    Curl* curl = NULL;

    if ((herr = handlesys->ReadHandle(handle, g_curl_type, &sec, (void**)curl))
            != HandleError_None) {  
        return pcontext->ThrowNativeError("Invalid curl handle %x (error %d)",
                params[1], herr);
    }

    char* url = NULL;
    ke::AString escaped;

    pcontext->LocalToString(params[2], &url);

    curl->UrlEncode(url, &escaped);

    pcontext->StringToLocal(params[3], params[4], escaped.chars());

    return 1;
}

//native bool:curl_exec(Handle:curl, CURL_CompleteCallback:func, any:userdata);
static cell_t CurlExec_SM(IPluginContext* pcontext, const cell_t* params) {  
    Handle_t handle = static_cast<Handle_t>(params[1]);
    HandleError herr = HandleError_None;
    HandleSecurity sec(pcontext->GetIdentity(), myself->GetIdentity());

    Curl* curl = NULL;

    if ((herr = handlesys->ReadHandle(handle, g_curl_type, &sec, (void**)curl))
            != HandleError_None) {  
        return pcontext->ThrowNativeError("Invalid curl handle %x (error %d)",
                params[1], herr);
    }

    IPluginFunction* pfunc = pcontext->GetFunctionById(params[2]);

    if (pfunc == NULL) {	
        return pcontext->ThrowNativeError("Invalid function id (%X)", params[2]);
    }

    CURLcode error = CURLE_OK;

    if (!curl->Exec()) {
        error = curl->last_error();
    }

    ExecuteCompleteCallback(pfunc, handle, error, params[3]);

    return error == CURLE_OK;
}

//native Handle:curl_init(const String:url[] = "");
static cell_t CurlInit_SM(IPluginContext* pcontext, const cell_t* params) {  
    Curl* curl = Curl::Initialize();

    if (curl == NULL) {  
        return pcontext->ThrowNativeError("Couldn't create curl handle");
    }

    char* url = NULL;
    pcontext->LocalToString(params[1], &url);

    if (url && strlen(url)) {	
        curl->SetOptionString(CURLOPT_URL, url);
    }

    Handle_t handle = handlesys->CreateHandle(
            g_curl_type, 
            &curl, 
            pcontext->GetIdentity(), 
            myself->GetIdentity(),  
            NULL);

    return handle;
}

//native curl_reset(Handle:curl);
static cell_t CurlReset_SM(IPluginContext* pcontext, const cell_t* params) {  
    Handle_t handle = static_cast<Handle_t>(params[1]);
    HandleError herr = HandleError_None;
    HandleSecurity sec(pcontext->GetIdentity(), myself->GetIdentity());

    Curl* curl = NULL;

    if ((herr = handlesys->ReadHandle(handle, g_curl_type, &sec, (void**)curl))
            != HandleError_None) {  
        return pcontext->ThrowNativeError("Invalid curl handle %x (error %d)",
                params[1], herr);
    }

    curl->Reset();

    return 1;
}

//native bool:curl_setopt_cell(Handle:curl, CURLoption option, value);
static cell_t CurlSetOptCell_SM(IPluginContext* pcontext, 
        const cell_t* params) {  
    CURLoption option = static_cast<CURLoption>(params[2]);

    if (!curl_ext_is_cell_option(option)) {
        return pcontext->ThrowNativeError(
                "curl_setopt_cell expects a long(cell) option");
    }

    Handle_t handle = static_cast<Handle_t>(params[1]);
    HandleError herr = HandleError_None;
    HandleSecurity sec(pcontext->GetIdentity(), myself->GetIdentity());

    Curl* curl = NULL;

    if ((herr = handlesys->ReadHandle(handle, g_curl_type, &sec, (void**)curl))
            != HandleError_None) {  
        return pcontext->ThrowNativeError("Invalid curl handle %x (error %d)",
                params[1], herr);
    }

    return curl->SetOptionInteger(option, params[3]) == true;
}


//native bool:curl_setopt_handle(Handle:curl, CURLoption option, Handle:handle);
static cell_t CurlSetOptHandle_SM(IPluginContext* pcontext, 
        const cell_t* params) {	
    CURLoption option = static_cast<CURLoption>(params[2]);

    if (!curl_ext_is_handle_option(option)) {
        return pcontext->ThrowNativeError(
                "curl_setopt_handle expects a handle option");
    }

    Handle_t handle = static_cast<Handle_t>(params[1]);
    HandleError herr = HandleError_None;
    HandleSecurity sec(pcontext->GetIdentity(), myself->GetIdentity());

    Curl* curl = NULL;

    if ((herr = handlesys->ReadHandle(handle, g_curl_type, &sec, (void**)curl))
            != HandleError_None) {  
        return pcontext->ThrowNativeError("Invalid curl handle %x (error %d)",
                params[1], herr);
    }

    /**
     * Convert Handle:value to C++ types and then set option with this value
     */

    switch(option) {
        case CURLOPT_WRITEDATA : 
            {
                break;
            }

        case CURLOPT_READDATA : 
            {
                break;
            }

        case CURLOPT_WRITEHEADER :
            {
                break;
            }

        case CURLOPT_HTTPHEADER : 
            {
                break;
            }

        case CURLOPT_HTTPPOST : 
            {
                break;
            }

        case CURLOPT_COPYPOSTFIELDS : 
            {
                break;
            } 

        case CURLOPT_POSTFIELDS : 
            {
                break;
            }

        default : 
            {
                return pcontext->ThrowNativeError("Option %d is not supported", option);
            }
    }

    return curl->SetOptionHandle(option, NULL) == true;
}


//native bool:curl_setopt_string(Handle:curl, CURLoption option, const String:value[]);
static cell_t CurlSetOptString_SM(IPluginContext* pcontext, 
        const cell_t* params) {
    CURLoption option = static_cast<CURLoption>(params[2]);

    if (!curl_ext_is_string_option(option)) {
        return pcontext->ThrowNativeError(
                "curl_setopt_string expects a string option");
    }

    Handle_t handle = static_cast<Handle_t>(params[1]);
    HandleError herr = HandleError_None;
    HandleSecurity sec(pcontext->GetIdentity(), myself->GetIdentity());

    Curl* curl = NULL;

    if ((herr = handlesys->ReadHandle(handle, g_curl_type, &sec, (void**)curl))
            != HandleError_None) {  
        return pcontext->ThrowNativeError("Invalid curl handle %x (error %d)",
                params[1], herr);
    }

    char* value = NULL;
    pcontext->LocalToString(params[3], &value);

    if (!value || !strlen(value)) {	
        return pcontext->ThrowNativeError(
                "Curl option contains invalid characters (\\0)");
    }

    return curl->SetOptionString(option, value) == true;
}

//native curl_thread_exec(Handle:curl, CURL_CompleteCallback:func, any:userdata);	 
static cell_t CurlThreadExec_SM(IPluginContext* pcontext, 
        const cell_t* params) {  
    Handle_t handle = static_cast<Handle_t>(params[1]);
    HandleError herr = HandleError_None;
    HandleSecurity sec(pcontext->GetIdentity(), myself->GetIdentity());

    Curl* curl = NULL;

    if ((herr = handlesys->ReadHandle(handle, g_curl_type, &sec, (void**)curl))
            != HandleError_None) {  
        return pcontext->ThrowNativeError("Invalid curl handle %x (error %d)",
                params[1], herr);
    }

    CompletePack* ppack = new CompletePack;

    ppack->pfunc = pcontext->GetFunctionById(params[2]);

    if (ppack->pfunc == NULL) {	
        return pcontext->ThrowNativeError("Invalid function id (%X)", params[2]);
    }

    ppack->handle = handle;
    ppack->error = CURLE_OK;
    ppack->user_data = params[3];

    /**
     * We have use one handle for every thread we plan to use for transferring. 
     * We must never share the same handle in multiple threads.
     *  
     * DECISION : duplicate curl handle
     * 
     * It is useful in multithreaded applications when you can run
     * curl_easy_duphandle() for each new thread to avoid a series of identical
     * curl_easy_setopt() invokes in every thread.
     * 
     * see (http://curl.haxx.se/libcurl/c/libcurl-tutorial.html)
     */

    Curl* duplicate = curl->MakeDuplicate(); 

    g_thread_pool->AddWork(new CurlThreadExecuter(duplicate, ppack));

    return 1;
}

const sp_nativeinfo_t kExtensionNatives[] = 
{
    {"curl_copy_handle",            CurlCopyHandle_SM},
    {"curl_errno",                  CurlErrNo_SM},
    {"curl_error",                  CurlError_SM},
    {"curl_escape",                 CurlEscape_SM},
    {"curl_exec",                   CurlExec_SM},
    {"curl_init",                   CurlInit_SM},
    {"curl_reset",                  CurlReset_SM},
    {"curl_setopt_cell",            CurlSetOptCell_SM},
    {"curl_setopt_handle",          CurlSetOptHandle_SM},
    {"curl_setopt_string",          CurlSetOptString_SM},
    {"curl_thread_exec",            CurlThreadExec_SM},
    {NULL, NULL},
};
