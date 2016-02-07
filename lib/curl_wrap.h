#ifndef CURL_MODULE_CURL_WRAP_H
#define CURL_MODULE_CURL_WRAP_H

#include "curl/curl.h"
#include "am-string.h"
#include "am-linkedlist.h"
#include "opts.h"

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

enum CurlMethodT {
    CURL_FILE,
    CURL_RETURN,
    CURL_FROM,
    CURL_IGNORE
};

struct CurlWrite {
    CurlMethodT method;
    ke::AString* buffer;
    FILE* file;
};

struct CurlRead {
    CurlMethodT method;
    ke::AString* buffer;
    FILE* file;
};

class CurlOption {
public:
    CurlOption(CURLoption opt, ke::AString* sval);
    CurlOption(CURLoption opt, int ival);
    CurlOption(CURLoption opt, void* hval);
    ~CurlOption();

    CURLoption opt;
    union {
        ke::AString* sval;
        int ival;
        void* hval;
    };
    enum {
        CELL = CURL_OPT_CELL,
        STRING = CURL_OPT_STRING,
        HANDLE = CURL_OPT_HANDLE
    } tag;
};

typedef ke::LinkedList<CurlOption*> CurlOptsListT;

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
    void ClearOpts();

    inline const char* StringifyLastError() {
        return curl_easy_strerror(last_error_);
    }

    CURLcode SetOptionHandle(CURLoption option, void* handle);
    CURLcode SetOptionCell(CURLoption option, int param);
    CURLcode SetOptionString(CURLoption option, const char* str);

    inline CURL* curl() {
        return curl_;
    }

    inline void set_write_data(CurlWrite* write_data) {
        write_data_ = write_data;
    }

    inline void set_read_data(CurlRead* read_data) {
        read_data_ = read_data;
    }

    inline void set_last_error(CURLcode error) {
        last_error_ = error;
    }

    inline CurlRead* read_data() {
        return read_data_;
    }

    inline CurlWrite* write_data() {
        return write_data_;
    }

    void UrlEncode(const char* url, ke::AString* out);
    void UrlDecode(const char* url, ke::AString* out);
private:
    CURL* curl_;
    CurlWrite* write_data_;
    CurlRead* read_data_;
    CURLcode last_error_;
    CurlOptsListT opts_;

    Curl(const Curl&);
    void operator=(const Curl&);
};

#endif
