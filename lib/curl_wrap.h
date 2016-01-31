#ifndef CURL_MODULE_CURL_WRAP_H
#define CURL_MODULE_CURL_WRAP_H

#include "curl/curl.h"
#include "am-string.h"
#include "opts.h"

bool inline curl_ext_is_option(CURLoption option, int type) {
#define _(op, t) (op == option && type == t) ||
    return SUPPORTED_OPT_LIST(_) /*||*/ false;
#undef _
}

extern bool inline curl_ext_is_cell_option(CURLoption option) {
    return curl_ext_is_option(option, CURL_EXT_CELL);
}

extern bool inline curl_ext_is_handle_option(CURLoption option) {
    return curl_ext_is_option(option, CURL_EXT_HANDLE);
}

extern bool inline curl_ext_is_string_option(CURLoption option) {
    return curl_ext_is_option(option, CURL_EXT_STRING);
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

class Curl
{
public:
    Curl(CURL* curl);
    ~Curl();

    static Curl* Initialize();

    CURLcode last_error() const {
        return last_error_;
    }

    Curl* MakeDuplicate();
    bool Exec();
    void Reset();

    inline const char* StringifyLastError() {
        return curl_easy_strerror(last_error_);
    }

    bool SetOptionHandle(CURLoption option, void* handle);
    bool SetOptionInteger(CURLoption option, int /*int32_t*/ param);
    bool SetOptionString(CURLoption option, const char* str);

    inline void set_write_data(CurlWrite* write_data) {
        write_data_ = write_data;
    }

    inline void set_read_data(CurlRead* read_data) {
        read_data_ = read_data;
    }

    inline void set_last_error(CURLcode error) {
        last_error_ = error;
    }

    inline CurlWrite* write_data() {
        return write_data_;
    }

    inline CurlRead* read_data() {
        return read_data_;
    }

    void UrlEncode(const char* url, ke::AString* out);
    void UrlDecode(const char* url, ke::AString* out);
private:
    CURL* curl_;
    CurlWrite* write_data_;
    CurlRead* read_data_;
    CURLcode last_error_;

    Curl(const Curl&);
    void operator=(const Curl&);
};

#endif
