#ifndef CURL_EXT_UTILS_SUPPORTED_OPT_H
#define CURL_EXT_UTILS_SUPPORTED_OPT_H

#define CURL_EXT_HANDLE 0
#define CURL_EXT_CELL 1
#define CURL_EXT_STRING 2

#define SUPPORTED_OPT_LIST(_)                                                  \
/*bool options*/                                                               \
  _(CURLOPT_COOKIESESSION,            CURL_EXT_CELL)                           \
  _(CURLOPT_AUTOREFERER,              CURL_EXT_CELL)                           \
  _(CURLOPT_CRLF,                     CURL_EXT_CELL)                           \
  _(CURLOPT_DNS_USE_GLOBAL_CACHE,     CURL_EXT_CELL)                           \
  _(CURLOPT_FAILONERROR,              CURL_EXT_CELL)                           \
  _(CURLOPT_FOLLOWLOCATION,           CURL_EXT_CELL)                           \
  _(CURLOPT_FORBID_REUSE,             CURL_EXT_CELL)                           \
  _(CURLOPT_FRESH_CONNECT,            CURL_EXT_CELL)                           \
  _(CURLOPT_HEADER,                   CURL_EXT_CELL)                           \
  _(CURLOPT_HTTPGET,                  CURL_EXT_CELL)                           \
  _(CURLOPT_HTTPPROXYTUNNEL,          CURL_EXT_CELL)                           \
/*_(CURLOPT_MUTE,                     CURL_EXT_CELL)*/                         \
  _(CURLOPT_NOBODY,                   CURL_EXT_CELL)                           \
  _(CURLOPT_POST,                     CURL_EXT_CELL)                           \
  _(CURLOPT_NOSIGNAL,                 CURL_EXT_CELL)                           \
  _(CURLOPT_PUT,                      CURL_EXT_CELL)                           \
  _(CURLOPT_UNRESTRICTED_AUTH,        CURL_EXT_CELL)                           \
  _(CURLOPT_UPLOAD,                   CURL_EXT_CELL)                           \
  _(CURLOPT_VERBOSE,                  CURL_EXT_CELL)                           \
/*cell options*/                                                               \
  _(CURLOPT_CONNECTTIMEOUT,           CURL_EXT_CELL)                           \
  _(CURLOPT_CONNECTTIMEOUT_MS,        CURL_EXT_CELL)                           \
  _(CURLOPT_DNS_CACHE_TIMEOUT,        CURL_EXT_CELL)                           \
  _(CURLOPT_HTTP_VERSION,             CURL_EXT_CELL)                           \
  _(CURLOPT_HTTPAUTH,                 CURL_EXT_CELL)                           \
  _(CURLOPT_INFILESIZE,               CURL_EXT_CELL)                           \
  _(CURLOPT_LOW_SPEED_LIMIT,          CURL_EXT_CELL)                           \
  _(CURLOPT_LOW_SPEED_TIME,           CURL_EXT_CELL)                           \
  _(CURLOPT_MAXCONNECTS,              CURL_EXT_CELL)                           \
  _(CURLOPT_PORT,                     CURL_EXT_CELL)                           \
  _(CURLOPT_PROXYAUTH,                CURL_EXT_CELL)                           \
  _(CURLOPT_PROXYPORT,                CURL_EXT_CELL)                           \
  _(CURLOPT_PROXYTYPE,                CURL_EXT_CELL)                           \
/*_(CURLOPT_REDIR_PROTOCOLS,          CURL_EXT_CELL)*/                         \
  _(CURLOPT_RESUME_FROM,              CURL_EXT_CELL)                           \
  _(CURLOPT_TIMEOUT,                  CURL_EXT_CELL)                           \
  _(CURLOPT_TIMEOUT_MS,               CURL_EXT_CELL)                           \
/*string options*/                                                             \
  _(CURLOPT_COOKIE,                   CURL_EXT_STRING)                         \
  _(CURLOPT_COOKIEFILE,               CURL_EXT_STRING)                         \
  _(CURLOPT_COOKIEJAR,                CURL_EXT_STRING)                         \
  _(CURLOPT_CUSTOMREQUEST,            CURL_EXT_STRING)                         \
  _(CURLOPT_ENCODING,                 CURL_EXT_STRING)                         \
  _(CURLOPT_INTERFACE,                CURL_EXT_STRING)                         \
  _(CURLOPT_PROXY,                    CURL_EXT_STRING)                         \
  _(CURLOPT_PROXYUSERPWD,             CURL_EXT_STRING)                         \
  _(CURLOPT_REFERER,                  CURL_EXT_STRING)                         \
  _(CURLOPT_URL,                      CURL_EXT_STRING)                         \
  _(CURLOPT_USERAGENT,                CURL_EXT_STRING)                         \
  _(CURLOPT_USERPWD,                  CURL_EXT_STRING)                         \
/*handle options*/                                                             \
  _(CURLOPT_POSTFIELDS,               CURL_EXT_HANDLE)                         \
  _(CURLOPT_WRITEDATA,                CURL_EXT_HANDLE)                         \
  _(CURLOPT_READDATA,                 CURL_EXT_HANDLE)                         \
  _(CURLOPT_WRITEHEADER,              CURL_EXT_HANDLE)                         \
  _(CURLOPT_HTTPHEADER,               CURL_EXT_HANDLE)                         \
  _(CURLOPT_HTTP200ALIASES,           CURL_EXT_HANDLE)                         \
  _(CURLOPT_HTTPPOST,                 CURL_EXT_HANDLE)                         \
  _(CURLOPT_COPYPOSTFIELDS,           CURL_EXT_HANDLE)                         


#endif // CURL_EXT_UTILS_SUPPORTED_OPT_H
