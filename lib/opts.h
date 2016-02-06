#ifndef CURL_MODULE_CURL_WRAP_OPT_H
#define CURL_MODULE_CURL_WRAP_OPT_H

#define CURL_OPT_HANDLE 0
#define CURL_OPT_CELL 1
#define CURL_OPT_STRING 2

/*
 * TODO separate long opt
 */

#define SUPPORTED_OPT_LIST(_)                                                  \
/*bool options*/                                                               \
  _(CURLOPT_COOKIESESSION,            CURL_OPT_CELL)                           \
  _(CURLOPT_AUTOREFERER,              CURL_OPT_CELL)                           \
  _(CURLOPT_CRLF,                     CURL_OPT_CELL)                           \
  _(CURLOPT_DNS_USE_GLOBAL_CACHE,     CURL_OPT_CELL)                           \
  _(CURLOPT_FAILONERROR,              CURL_OPT_CELL)                           \
  _(CURLOPT_FOLLOWLOCATION,           CURL_OPT_CELL)                           \
  _(CURLOPT_FORBID_REUSE,             CURL_OPT_CELL)                           \
  _(CURLOPT_FRESH_CONNECT,            CURL_OPT_CELL)                           \
  _(CURLOPT_HEADER,                   CURL_OPT_CELL)                           \
  _(CURLOPT_HTTPGET,                  CURL_OPT_CELL)                           \
  _(CURLOPT_HTTPPROXYTUNNEL,          CURL_OPT_CELL)                           \
/*_(CURLOPT_MUTE,                     CURL_OPT_CELL)*/                         \
  _(CURLOPT_NOBODY,                   CURL_OPT_CELL)                           \
  _(CURLOPT_POST,                     CURL_OPT_CELL)                           \
  _(CURLOPT_NOSIGNAL,                 CURL_OPT_CELL)                           \
  _(CURLOPT_PUT,                      CURL_OPT_CELL)                           \
  _(CURLOPT_UNRESTRICTED_AUTH,        CURL_OPT_CELL)                           \
  _(CURLOPT_UPLOAD,                   CURL_OPT_CELL)                           \
  _(CURLOPT_VERBOSE,                  CURL_OPT_CELL)                           \
/*cell options*/                                                               \
  _(CURLOPT_CONNECTTIMEOUT,           CURL_OPT_CELL)                           \
  _(CURLOPT_CONNECTTIMEOUT_MS,        CURL_OPT_CELL)                           \
  _(CURLOPT_DNS_CACHE_TIMEOUT,        CURL_OPT_CELL)                           \
  _(CURLOPT_HTTP_VERSION,             CURL_OPT_CELL)                           \
  _(CURLOPT_HTTPAUTH,                 CURL_OPT_CELL)                           \
  _(CURLOPT_INFILESIZE,               CURL_OPT_CELL)                           \
  _(CURLOPT_LOW_SPEED_LIMIT,          CURL_OPT_CELL)                           \
  _(CURLOPT_LOW_SPEED_TIME,           CURL_OPT_CELL)                           \
  _(CURLOPT_MAXCONNECTS,              CURL_OPT_CELL)                           \
  _(CURLOPT_PORT,                     CURL_OPT_CELL)                           \
  _(CURLOPT_PROXYAUTH,                CURL_OPT_CELL)                           \
  _(CURLOPT_PROXYPORT,                CURL_OPT_CELL)                           \
  _(CURLOPT_PROXYTYPE,                CURL_OPT_CELL)                           \
/*_(CURLOPT_REDIR_PROTOCOLS,          CURL_OPT_CELL)*/                         \
  _(CURLOPT_RESUME_FROM,              CURL_OPT_CELL)                           \
  _(CURLOPT_TIMEOUT,                  CURL_OPT_CELL)                           \
  _(CURLOPT_TIMEOUT_MS,               CURL_OPT_CELL)                           \
/*string options*/                                                             \
  _(CURLOPT_COOKIE,                   CURL_OPT_STRING)                         \
  _(CURLOPT_COOKIEFILE,               CURL_OPT_STRING)                         \
  _(CURLOPT_COOKIEJAR,                CURL_OPT_STRING)                         \
  _(CURLOPT_CUSTOMREQUEST,            CURL_OPT_STRING)                         \
  _(CURLOPT_ENCODING,                 CURL_OPT_STRING)                         \
  _(CURLOPT_INTERFACE,                CURL_OPT_STRING)                         \
  _(CURLOPT_PROXY,                    CURL_OPT_STRING)                         \
  _(CURLOPT_PROXYUSERPWD,             CURL_OPT_STRING)                         \
  _(CURLOPT_REFERER,                  CURL_OPT_STRING)                         \
  _(CURLOPT_URL,                      CURL_OPT_STRING)                         \
  _(CURLOPT_USERAGENT,                CURL_OPT_STRING)                         \
  _(CURLOPT_USERPWD,                  CURL_OPT_STRING)                         \
/*handle options*/                                                             \
  _(CURLOPT_POSTFIELDS,               CURL_OPT_HANDLE)                         \
  _(CURLOPT_WRITEDATA,                CURL_OPT_HANDLE)                         \
  _(CURLOPT_READDATA,                 CURL_OPT_HANDLE)                         \
  _(CURLOPT_WRITEHEADER,              CURL_OPT_HANDLE)                         \
  _(CURLOPT_HTTPHEADER,               CURL_OPT_HANDLE)                         \
  _(CURLOPT_HTTP200ALIASES,           CURL_OPT_HANDLE)                         \
  _(CURLOPT_HTTPPOST,                 CURL_OPT_HANDLE)                         \
  _(CURLOPT_COPYPOSTFIELDS,           CURL_OPT_HANDLE)


#endif
