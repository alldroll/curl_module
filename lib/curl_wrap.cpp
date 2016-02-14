#include "curl_wrap.h"

static size_t ReadFunction(
    char* data, size_t bytes, size_t nitems, void* ctx
) {
    size_t length = 0;
    Curl* curl = (Curl*) ctx;
    CurlRead& curl_read = curl->read_data();

    switch (curl_read.method) {
        case CURL_FILE:
            return fread(data, bytes, nitems, curl_read.file);

        case CURL_FROM: { /* TODO */ break; }

        default: break; /* ignore case */
    }

    return length;
}

static size_t WriteFunction(
    char* data, size_t bytes, size_t nitems, void* ctx
) {
    size_t length = bytes * nitems;
    Curl* curl = (Curl*) ctx;
    CurlWrite& curl_write = curl->write_data();

    switch (curl_write.method) {
        case CURL_FILE:
            return fwrite(data, bytes, nitems, curl_write.file);

        case CURL_RETURN: {
            BufferT* buffer = &curl_write.buffer;
            int offset = buffer->size;
            if (offset == BUFFER_SIZE) {
                curl_write.Flush();
            }

            snprintf(buffer->buf + offset, BUFFER_SIZE - offset, data);
            buffer->size += length < BUFFER_SIZE ? length: BUFFER_SIZE;
        }

        default: break; /* ignore case */
    }

    return length;
}

static void SetDefaultOptions(Curl* ctx) {
    if (ctx == NULL) {
        return;
    }

    CURL* h = ctx->curl();

    curl_easy_setopt(h, CURLOPT_READFUNCTION, ReadFunction);
    curl_easy_setopt(h, CURLOPT_WRITEFUNCTION, WriteFunction);
    curl_easy_setopt(h, CURLOPT_WRITEDATA, ctx);
    curl_easy_setopt(h, CURLOPT_READDATA, ctx);

    curl_easy_setopt(h, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(h, CURLOPT_NOSIGNAL, 1);
}

CurlOption::CurlOption(CURLoption opt, ke::AString* sval) :
    opt(opt), sval(sval), tag(CurlOption::STRING) {}

CurlOption::CurlOption(CURLoption opt, int ival) :
    opt(opt), ival(ival), tag(CurlOption::CELL) {}

CurlOption::CurlOption(CURLoption opt, void* hval) :
    opt(opt), hval(hval), tag(CurlOption::HANDLE) {}

CurlOption::~CurlOption() {
    if (tag == CurlOption::STRING && sval) {
        delete sval;
    }
}

CurlRead::CurlRead() : method(CURL_IGNORE), file(NULL) {}

CurlWrite::CurlWrite() : method(CURL_RETURN), file(NULL) {
    buffer.size = 0;
}

void CurlWrite::Flush() {
    if (method == CURL_RETURN) {
        buffer.size = 0;
    }
}

CurlWrite::CurlWrite(CurlWrite& other) {
    //buffer = other.buffer; TODO
    method = other.method;
    file = other.file;
}

Curl::Curl(CURL* curl) : curl_(curl), last_error_(CURLE_OK) {}

Curl::~Curl() {
    if (curl_ != NULL) {
        curl_easy_cleanup(curl_);
    }
    ClearOpts();
}

Curl* Curl::Initialize() {
    CURL* h = curl_easy_init();

    if (h == NULL) {
        return NULL;
    }

    Curl* curl = new Curl(h);
    SetDefaultOptions(curl);
    return curl;
}

CURLcode Curl::SetOptionHandle(CURLoption option, void* handle) {
    return CURLE_OK;
}

CURLcode Curl::SetOptionCell(CURLoption option, int value) {
    CURLcode code = CURLE_OK;

    switch (option) {
        case CURLOPT_COOKIESESSION:
        case CURLOPT_AUTOREFERER:
        case CURLOPT_CRLF:
        case CURLOPT_DNS_USE_GLOBAL_CACHE:
        case CURLOPT_FAILONERROR:
        case CURLOPT_FOLLOWLOCATION:
        case CURLOPT_FORBID_REUSE:
        case CURLOPT_FRESH_CONNECT:
        case CURLOPT_HEADER:
        case CURLOPT_HTTPGET:
        case CURLOPT_HTTPPROXYTUNNEL:
        case CURLOPT_NOBODY:
        case CURLOPT_POST:
        case CURLOPT_NOSIGNAL:
        case CURLOPT_PUT:
        case CURLOPT_UNRESTRICTED_AUTH:
        case CURLOPT_UPLOAD:
        case CURLOPT_VERBOSE:
        case CURLOPT_CONNECTTIMEOUT:
        case CURLOPT_CONNECTTIMEOUT_MS:
        case CURLOPT_DNS_CACHE_TIMEOUT:
        case CURLOPT_HTTP_VERSION:
        case CURLOPT_HTTPAUTH:
        case CURLOPT_INFILESIZE:
        case CURLOPT_LOW_SPEED_LIMIT:
        case CURLOPT_LOW_SPEED_TIME:
        case CURLOPT_MAXCONNECTS:
        case CURLOPT_PORT:
        case CURLOPT_PROXYAUTH:
        case CURLOPT_PROXYPORT:
        case CURLOPT_PROXYTYPE:
        case CURLOPT_RESUME_FROM:
        case CURLOPT_TIMEOUT:
        case CURLOPT_TIMEOUT_MS: {
            CurlOption* pack = new CurlOption(option, value);
            opts_.prepend(pack);
            code = curl_easy_setopt(curl_, option, value);

            break;
        }
        default: break;
    }

    return code;
}

CURLcode Curl::SetOptionString(CURLoption option, const char* str) {
    CURLcode code = CURLE_OK;

    switch (option) {
        case CURLOPT_COOKIE:
        case CURLOPT_COOKIEFILE:
        case CURLOPT_COOKIEJAR:
        case CURLOPT_CUSTOMREQUEST:
        case CURLOPT_ENCODING:
        case CURLOPT_INTERFACE:
        case CURLOPT_PROXY:
        case CURLOPT_PROXYUSERPWD:
        case CURLOPT_REFERER:
        case CURLOPT_URL:
        case CURLOPT_USERAGENT:
        case CURLOPT_USERPWD: {
            CurlOption* pack = new CurlOption(option, new ke::AString(str));
            opts_.prepend(pack);
            code = curl_easy_setopt(curl_, option, pack->sval->chars());

            break;
        };
        default: break;
    }

    return code;
}

Curl* Curl::MakeDuplicate() {
    Curl* duplicate = Curl::Initialize();
    if (!duplicate) {
        return NULL;
    }

    duplicate->set_write_data(write_data_);
    duplicate->set_read_data(read_data_);
    duplicate->set_last_error(last_error_);

    CurlOptsListT::iterator iter = opts_.begin();

    while (iter != opts_.end()) {
        const CurlOption* val = *iter;

        switch (val->tag) {
            case CURL_OPT_CELL: {
                duplicate->SetOptionCell(val->opt, val->ival);
                break;
            }

            case CURL_OPT_STRING: {
                duplicate->SetOptionString(val->opt, val->sval->chars());
                break;
            }

            case CURL_OPT_HANDLE: {
                duplicate->SetOptionHandle(val->opt, val->hval);
                break;
            }

            default: break;
        }

        iter++;
    }

    return duplicate;
}

CURLcode Curl::Exec() {
    last_error_ = curl_easy_perform(curl_);
    return last_error_;
}

void Curl::ClearOpts() {
    CurlOptsListT::iterator iter = opts_.begin();
    while (iter != opts_.end()) {
        CurlOption* to_free = *iter;
        iter = opts_.erase(iter);
        delete to_free;
    }
}

void Curl::Reset() {
    curl_easy_reset(curl_);
    SetDefaultOptions(this);
    ClearOpts();
}

void Curl::UrlEncode(const char* url, ke::AString* out) {
    if (out == NULL) {
        return;
    }

    char* escaped = curl_easy_escape(curl_, url, 0);
    *out = escaped;
    curl_free(escaped);
}

void Curl::UrlDecode(const char* url, ke::AString* out) {
    if (out == NULL) {
        return;
    }

    char* unescaped = curl_easy_unescape(curl_, url, 0, NULL);
    *out = unescaped;
    curl_free(unescaped);
}
