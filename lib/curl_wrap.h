#ifndef CURL_MODULE_CURL_WRAP_H
#define CURL_MODULE_CURL_WRAP_H

#include "curl/curl.h"
#include "am-string.h"
#include "am-hashmap.h"
#include "opts.h"

#define BUFFER_SIZE 3072

bool inline curl_module_is_option(CURLoption option, int type) {
#define _(op, t) (op == option && type == t) ||
    return SUPPORTED_OPT_LIST(_) /*||*/ false;
#undef _
}

extern bool inline curl_module_is_cell_option(CURLoption option) {
    return curl_module_is_option(option, CURL_OPT_CELL);
}

extern bool inline curl_module_is_handle_option(CURLoption option) {
    return curl_module_is_option(option, CURL_OPT_HANDLE);
}

extern bool inline curl_module_is_string_option(CURLoption option) {
    return curl_module_is_option(option, CURL_OPT_STRING);
}

class CurlSList {
public:
    CurlSList(curl_slist* _slist) {
        slist = _slist;
    }
    ~CurlSList() {
        if (slist) {
            curl_slist_free_all(slist);
        }
    }

    curl_slist* slist;
};

class CurlWebForm {
public:
    CurlWebForm();
    ~CurlWebForm();

    CURLFORMcode get_last_error() {
        return last_error_;
    }

    curl_httppost* GetFormData();
    bool AddString(const char* name, const char* data);
    bool AddFile(const char* name, const char* path);
    /* todo addarray ... */

private:
    curl_httppost* first_;
    curl_httppost* last_;
    CURLFORMcode last_error_;
};

enum CurlMethodT {
    CURL_FILE,
    CURL_RETURN,
    CURL_FROM,
    CURL_IGNORE
};

struct BufferT {
    char buf[BUFFER_SIZE];
    int size;
};

class CurlWrite {
public:
    CurlWrite();
    CurlWrite(const CurlWrite&);
    void Flush();

    CurlMethodT method;
    BufferT buffer;
    FILE* file;
};

class CurlRead {
public:
    CurlRead();

    CurlMethodT method;
    BufferT buffer;
    FILE* file;
};

class Curl {
public:
    Curl(CURL* curl);
    ~Curl();

    static Curl* Initialize();

    CURLcode last_error() const {
        return last_error_;
    }

    Curl* MakeDuplicate();
    CURLcode Exec();
    void Reset();

    inline const char* StringifyLastError() {
        return curl_easy_strerror(last_error_);
    }

    CURLcode SetOptionHandle(CURLoption option, void* handle);
    CURLcode SetOptionCell(CURLoption option, int param);
    CURLcode SetOptionString(CURLoption option, const char* str);

    inline CURL* curl() {
        return curl_;
    }

    inline void set_write_data(const CurlWrite& write_data) {
        write_data_ = write_data;
    }

    inline void set_read_data(const CurlRead& read_data) {
        read_data_ = read_data;
    }

    inline void set_last_error(CURLcode error) {
        last_error_ = error;
    }

    inline CurlRead& read_data() {
        return read_data_;
    }

    inline CurlWrite& write_data() {
        return write_data_;
    }

    void UrlEncode(const char* url, ke::AString* out);
    void UrlDecode(const char* url, ke::AString* out);
private:
    CURL* curl_;
    CurlWrite write_data_;
    CurlRead read_data_;
    CURLcode last_error_;

    Curl(const Curl&);
    void operator=(const Curl&);
};

#endif
