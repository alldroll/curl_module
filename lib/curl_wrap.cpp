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

CurlWebForm::CurlWebForm():
    first_(NULL),
    last_(NULL),
    last_error_(CURL_FORMADD_OK) {}

CurlWebForm::~CurlWebForm() {
    if (first_) {
        curl_formfree(first_);
    }
}

curl_httppost* CurlWebForm::GetFormData() {
    return first_;
}

bool CurlWebForm::SetOptionCell(CURLformoption opt, int data) {
    last_error_ = curl_formadd(
        &first_,
        &last_,
        opt,
        data,
        CURLFORM_END
    );

    return last_error_ == CURL_FORMADD_OK;
}

bool CurlWebForm::SetOptionString(CURLformoption opt, const char* data) {
    last_error_ = curl_formadd(
        &first_,
        &last_,
        opt,
        data,
        CURLFORM_END
    );

    return last_error_ == CURL_FORMADD_OK;
}

bool CurlWebForm::SetOptionHandle(CURLformoption opt, void* data) {
    last_error_ = curl_formadd(
        &first_,
        &last_,
        opt,
        data,
        CURLFORM_END
    );

    return last_error_ == CURL_FORMADD_OK;
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

CurlWrite::CurlWrite(const CurlWrite& other) {
    method = other.method;
    file = other.file;
}

Curl::Curl(CURL* curl) : curl_(curl), last_error_(CURLE_OK) {
}

Curl::~Curl() {
    if (curl_ != NULL) {
        curl_easy_cleanup(curl_);
    }
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
    CURLcode code = CURLE_OK;
    switch (option) {
        case CURLOPT_HTTPHEADER: {
            CurlSList* wrapper = (CurlSList*) handle;
            code = curl_easy_setopt(curl_, option, wrapper->slist);
            break;
        }
    }

    return code;
}

CURLcode Curl::SetOptionCell(CURLoption option, int value) {
    return curl_easy_setopt(curl_, option, value);
}

CURLcode Curl::SetOptionString(CURLoption option, const char* str) {
    CURLcode code = CURLE_OK;

    switch (option) {
        case CURLOPT_POSTFIELDS: {
            /* force curl copy postfields */
            option = CURLOPT_COPYPOSTFIELDS;
            break;
        }

        default: break;
    }

    return curl_easy_setopt(curl_, option, str);
}

Curl* Curl::MakeDuplicate() {
    CURL* h = curl_easy_duphandle(curl_);
    if (h == NULL) {
        return NULL;
    }

    Curl* duplicate = new Curl(h);
    duplicate->set_write_data(write_data_);
    duplicate->set_read_data(read_data_);
    duplicate->set_last_error(last_error_);

    SetDefaultOptions(duplicate);

    return duplicate;
}

CURLcode Curl::Exec() {
    last_error_ = curl_easy_perform(curl_);
    return last_error_;
}

void Curl::Reset() {
    curl_easy_reset(curl_);
    SetDefaultOptions(this);
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
