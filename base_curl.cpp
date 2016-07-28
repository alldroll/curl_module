#include <stdlib.h>
#include "curl_header.h"
#include "curl_wrap.h"

static void FreeCurl(void* p, unsigned int num) {
    Curl* curl = (Curl*) p;
    delete curl;
}

static void FreeSList(void* p, unsigned int num) {
    CurlSList* slist = (CurlSList*) p;
    delete slist;
}

static void FreeForm(void* p, unsigned int num) {
    CurlWebForm* form = (CurlWebForm*) p;
    delete form;
}

// native Handle:curl_init();
static cell AMX_NATIVE_CALL AMX_CurlInit(AMX* amx, cell* params) {
    Curl* curl = Curl::Initialize();
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Couldn't alloc curl handle");
        return -1;
    }

    return MakeHandle(curl, HANDLE_CURL, FreeCurl);
}

// native curl_close(Handle:h);
static cell AMX_NATIVE_CALL AMX_CurlClose(AMX* amx, cell* params) {
    bool success = true;
    if (!FreeHandle(params[1])) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        success = false;
    }

    return success;
}

// native curl_reset(Handle:h);
static cell AMX_NATIVE_CALL AMX_CurlReset(AMX* amx, cell* params) {
    bool success = true;
    Curl* curl = (Curl*)GetHandle(params[1], HANDLE_CURL);
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        success = false;
    } else {
        curl->Reset();
    }

    return success;
}


/* native CURLcode:curl_errno(Handle:h);
 * TODO fix last_error is not actual
 */
static cell AMX_NATIVE_CALL AMX_CurlErrno(AMX* amx, cell* params) {
    Curl* curl = (Curl*)GetHandle(params[1], HANDLE_CURL);
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        return -1;
    }

    return curl->last_error();
}

// native CURLcode:curl_setopt_cell(Handle:h, CURLoption:opt, value)
static cell AMX_NATIVE_CALL AMX_CurlSetOptCell(AMX* amx, cell* params) {
    Curl* curl = (Curl*)GetHandle(params[1], HANDLE_CURL);
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        return -1;
    }

    CURLoption opt = (CURLoption)params[2];
    if (!curl_module_is_cell_option(opt)) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Unsupported cell option: %d", opt);
        return -1;
    }

    return curl->SetOptionCell(opt, (int)params[3]);
}

// native CURLcode:curl_setopt_string(Handle:h, CURLoption:opt, const val[])
static cell AMX_NATIVE_CALL AMX_CurlSetOptString(AMX* amx, cell* params) {
    Curl* curl = (Curl*)GetHandle(params[1], HANDLE_CURL);
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        return -1;
    }

    CURLoption opt = (CURLoption)params[2];
    if (!curl_module_is_string_option(opt)) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Unsupported string option: %d", opt);
        return -1;
    }

    int len;
    char* val = MF_GetAmxString(amx, params[3], 0, &len);
    return curl->SetOptionString(opt, val);
}

// native CURLcode:curl_setopt_handle(Handle:h, CURLoption:opt, Handle:val)
static cell AMX_NATIVE_CALL AMX_CurlSetOptHandle(AMX* amx, cell* params) {
    Curl* curl = (Curl*)GetHandle(params[1], HANDLE_CURL);
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        return -1;
    }

    CURLoption opt = (CURLoption)params[2];
    if (!curl_module_is_handle_option(opt)) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Unsupported handle option: %d", opt);
        return -1;
    }

    switch (opt) {
        case CURLOPT_HTTPHEADER: {
            void* val = GetHandle(params[3], HANDLE_CURL_SLIST);
            if (!val) {
                return -1;
            }

            return curl->SetOptionHandle(opt, val);
        }
    }

    return 0;
}

// native Handle:curl_duphandle(Handle:h)
static cell AMX_NATIVE_CALL AMX_CurlDupHandle(AMX* amx, cell* params) {
    Curl* curl = (Curl*)GetHandle(params[1], HANDLE_CURL);
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        return -1;
    }

    Curl* duplicate = curl->MakeDuplicate();
    if (!duplicate) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Couldn't allocate duplicate");
        return -1;
    }

    return MakeHandle(duplicate, HANDLE_CURL, FreeCurl);
}

// native curl_escape(Handle:curl, const val[], to[], length)
static cell AMX_NATIVE_CALL AMX_CurlEscape(AMX* amx, cell* params) {
    Curl* curl = (Curl*)GetHandle(params[1], HANDLE_CURL);
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        return -1;
    }

    int len;
    char* val = MF_GetAmxString(amx, params[2], 0, &len);
    ke::AString buffer;
    curl->UrlEncode(val, &buffer);

    return MF_SetAmxString(amx, params[3], buffer.chars(), params[4]);
}

// native curl_unescape(Handle:curl, const val[], to[], length)
static cell AMX_NATIVE_CALL AMX_CurlUnescape(AMX* amx, cell* params) {
    Curl* curl = (Curl*)GetHandle(params[1], HANDLE_CURL);
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        return -1;
    }

    int len;
    char* val = MF_GetAmxString(amx, params[2], 0, &len);
    ke::AString buffer;
    curl->UrlDecode(val, &buffer);

    return MF_SetAmxString(amx, params[3], buffer.chars(), params[4]);
}

/*
 * public OnComplete(Handle:curl, CURLcode:code, const response[], any:data)
 * native CURLcode:curl_exec(Handle:h, const callback[], any:data)
 */
static cell AMX_NATIVE_CALL AMX_CurlExec(AMX* amx, cell* params) {
    Curl* curl = (Curl*)GetHandle(params[1], HANDLE_CURL);
    if (!curl) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        return -1;
    }

    CURLcode code = curl->Exec();
    CurlWrite& write_data = curl->write_data();
    int len;
    const char* callback = MF_GetAmxString(amx, params[2], 0, &len);

    if (write_data.method == CURL_RETURN && strlen(callback) > 0) {
        int fwd = MF_RegisterSPForwardByName(
            amx, callback, FP_CELL, FP_CELL, FP_STRING, FP_CELL, FP_DONE
        );
        if (fwd < 1) {
            MF_LogError(amx, AMX_ERR_NATIVE, "Function not found: %s", callback);
            return -1;
        }

        MF_ExecuteForward(
            fwd,
            params[1],
            code,
            write_data.buffer.buf,
            params[3]
        );
        MF_UnregisterSPForward(fwd);
    }

    write_data.Flush();
    return code;
}

// native Handle:curl_create_slist()
static cell AMX_NATIVE_CALL AMX_CurlCreateSList(AMX* amx, cell* params) {
    CurlSList* slist = new CurlSList(NULL);
    if (!slist) { /* is it ok ? */
        MF_LogError(amx, AMX_ERR_NATIVE, "Couldn't alloc curl handle");
        return -1;
    }

    return MakeHandle(slist, HANDLE_CURL_SLIST, FreeSList);
}

// native curl_destroy_slist(Handle:slist)
static cell AMX_NATIVE_CALL AMX_CurlDestroySList(AMX* amx, cell* params) {
    bool success = true;
    if (!FreeHandle(params[1])) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        success = false;
    }

    return success;
}

// native curl_slist_append(Handle:slist, const str[])
static cell AMX_NATIVE_CALL AMX_CurlSListAppend(AMX* amx, cell* params) {
    CurlSList* handle = (CurlSList*)GetHandle(params[1], HANDLE_CURL_SLIST);
    if (!handle) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        return false;
    }

    int len;
    char* str = MF_GetAmxString(amx, params[2], 0, &len);
    handle->slist = curl_slist_append(handle->slist, str);
    return true;
}

// native Handle:curl_create_form()
static cell AMX_NATIVE_CALL AMX_CurlCreateForm(AMX* amx, cell* params) {
    CurlWebForm* form = new CurlWebForm();
    if (!form) { /* is it ok ? */
        MF_LogError(amx, AMX_ERR_NATIVE, "Couldn't alloc curl handle");
        return -1;
    }

    return MakeHandle(form, HANDLE_CURL_FORM, FreeForm);
}

// native curl_destroy_form(Handle:form)
static cell AMX_NATIVE_CALL AMX_CurlDestroyForm(AMX* amx, cell* params) {
    bool success = true;
    if (!FreeHandle(params[1])) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        success = false;
    }

    return success;
}

// native curl_form_add(Handle:form, any:...)
static cell AMX_NATIVE_CALL AMX_CurlFormAdd(AMX* amx, cell* params) {
    CurlWebForm* form = (CurlWebForm*)GetHandle(params[1], HANDLE_CURL_FORM);
    if (!form) {
        MF_LogError(amx, AMX_ERR_NATIVE, "Invalid handle: %d", params[1]);
        return false;
    }

    CURLFORMcode result = CURL_FORMADD_INCOMPLETE;
    unsigned int params_number = params[0], i = 2, j = 0;

    for (; i < params_number; ++i) {
        CURLformoption opt = (CURLformoption)params[i];

        if (CURLFORM_END == opt) {
            break;
        }

        if (curl_module_form_is_cell_option(opt)) {

        } else if (curl_module_form_is_string_option(opt)) {

        } else if (curl_module_form_is_handle_option(opt)) {

        } else {
            result = CURL_FORMADD_INCOMPLETE;
            break;
        }
    }

    return result;
}

AMX_NATIVE_INFO g_BaseCurlNatives[] = {
    {"curl_init", AMX_CurlInit},
    {"curl_close", AMX_CurlClose},
    {"curl_reset", AMX_CurlReset},
    {"curl_errno", AMX_CurlErrno},
    {"curl_setopt_cell", AMX_CurlSetOptCell},
    {"curl_setopt_string", AMX_CurlSetOptString},
    {"curl_setopt_handle", AMX_CurlSetOptHandle},
    {"curl_exec", AMX_CurlExec},
    {"curl_duphandle", AMX_CurlDupHandle},
    {"curl_escape", AMX_CurlEscape},
    {"curl_unescape", AMX_CurlUnescape},
    {"curl_create_slist", AMX_CurlCreateSList},
    {"curl_destroy_slist", AMX_CurlDestroySList},
    {"curl_slist_append", AMX_CurlSListAppend},
    {"curl_create_form", AMX_CurlCreateForm},
    {"curl_destroy_form", AMX_CurlDestroyForm},
    {NULL, NULL}
};
