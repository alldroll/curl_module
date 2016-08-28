#if defined _curl_included
    #endinput
#endif
#define _curl_included

#pragma reqlib curl
#if !defined AMXMODX_NOAUTOLOAD
    #pragma loadlib curl
#endif

enum Handle
{
   INVALID = INVALID_HANDLE,
};

enum CURLcode
{
    INVALID = INVALID_HANDLE,

    CURLE_OK = 0,
    CURLE_UNSUPPORTED_PROTOCOL,    /* 1 */
    CURLE_FAILED_INIT,             /* 2 */
    CURLE_URL_MALFORMAT,           /* 3 */
    CURLE_OBSOLETE4,               /* 4 - NOT USED */
    CURLE_COULDNT_RESOLVE_PROXY,   /* 5 */
    CURLE_COULDNT_RESOLVE_HOST,    /* 6 */
    CURLE_COULDNT_CONNECT,         /* 7 */
    CURLE_FTP_WEIRD_SERVER_REPLY,  /* 8 */
    CURLE_REMOTE_ACCESS_DENIED,    /* 9 a service was denied by the server
                                    due to lack of access - when login fails
                                    this is not returned. */
    CURLE_OBSOLETE10,              /* 10 - NOT USED */
    CURLE_FTP_WEIRD_PASS_REPLY,    /* 11 */
    CURLE_OBSOLETE12,              /* 12 - NOT USED */
    CURLE_FTP_WEIRD_PASV_REPLY,    /* 13 */
    CURLE_FTP_WEIRD_227_FORMAT,    /* 14 */
    CURLE_FTP_CANT_GET_HOST,       /* 15 */
    CURLE_OBSOLETE16,              /* 16 - NOT USED */
    CURLE_FTP_COULDNT_SET_TYPE,    /* 17 */
    CURLE_PARTIAL_FILE,            /* 18 */
    CURLE_FTP_COULDNT_RETR_FILE,   /* 19 */
    CURLE_OBSOLETE20,              /* 20 - NOT USED */
    CURLE_QUOTE_ERROR,             /* 21 - quote command failure */
    CURLE_HTTP_RETURNED_ERROR,     /* 22 */
    CURLE_WRITE_ERROR,             /* 23 */
    CURLE_OBSOLETE24,              /* 24 - NOT USED */
    CURLE_UPLOAD_FAILED,           /* 25 - failed upload "command" */
    CURLE_READ_ERROR,              /* 26 - couldn't open/read from file */
    CURLE_OUT_OF_MEMORY,           /* 27 */
    /* Note: CURLE_OUT_OF_MEMORY may sometimes indicate a conversion error
           instead of a memory allocation error if CURL_DOES_CONVERSIONS
           is defined
    */
    CURLE_OPERATION_TIMEDOUT,      /* 28 - the timeout time was reached */
    CURLE_OBSOLETE29,              /* 29 - NOT USED */
    CURLE_FTP_PORT_FAILED,         /* 30 - FTP PORT operation failed */
    CURLE_FTP_COULDNT_USE_REST,    /* 31 - the REST command failed */
    CURLE_OBSOLETE32,              /* 32 - NOT USED */
    CURLE_RANGE_ERROR,             /* 33 - RANGE "command" didn't work */
    CURLE_HTTP_POST_ERROR,         /* 34 */
    CURLE_SSL_CONNECT_ERROR,       /* 35 - wrong when connecting with SSL */
    CURLE_BAD_DOWNLOAD_RESUME,     /* 36 - couldn't resume download */
    CURLE_FILE_COULDNT_READ_FILE,  /* 37 */
    CURLE_LDAP_CANNOT_BIND,        /* 38 */
    CURLE_LDAP_SEARCH_FAILED,      /* 39 */
    CURLE_OBSOLETE40,              /* 40 - NOT USED */
    CURLE_FUNCTION_NOT_FOUND,      /* 41 */
    CURLE_ABORTED_BY_CALLBACK,     /* 42 */
    CURLE_BAD_FUNCTION_ARGUMENT,   /* 43 */
    CURLE_OBSOLETE44,              /* 44 - NOT USED */
    CURLE_INTERFACE_FAILED,        /* 45 - CURLOPT_INTERFACE failed */
    CURLE_OBSOLETE46,              /* 46 - NOT USED */
    CURLE_TOO_MANY_REDIRECTS ,     /* 47 - catch endless re-direct loops */
    CURLE_UNKNOWN_TELNET_OPTION,   /* 48 - User specified an unknown option */
    CURLE_TELNET_OPTION_SYNTAX ,   /* 49 - Malformed telnet option */
    CURLE_OBSOLETE50,              /* 50 - NOT USED */
    CURLE_PEER_FAILED_VERIFICATION, /* 51 - peer's certificate or fingerprint
                                     wasn't verified fine */
    CURLE_GOT_NOTHING,             /* 52 - when this is a specific error */
    CURLE_SSL_ENGINE_NOTFOUND,     /* 53 - SSL crypto engine not found */
    CURLE_SSL_ENGINE_SETFAILED,    /* 54 - can not set SSL crypto engine as
                                    default */
    CURLE_SEND_ERROR,              /* 55 - failed sending network data */
    CURLE_RECV_ERROR,              /* 56 - failure in receiving network data */
    CURLE_OBSOLETE57,              /* 57 - NOT IN USE */
    CURLE_SSL_CERTPROBLEM,         /* 58 - problem with the local certificate */
    CURLE_SSL_CIPHER,              /* 59 - couldn't use specified cipher */
    CURLE_SSL_CACERT,              /* 60 - problem with the CA cert (path?) */
    CURLE_BAD_CONTENT_ENCODING,    /* 61 - Unrecognized transfer encoding */
    CURLE_LDAP_INVALID_URL,        /* 62 - Invalid LDAP URL */
    CURLE_FILESIZE_EXCEEDED,       /* 63 - Maximum file size exceeded */
    CURLE_USE_SSL_FAILED,          /* 64 - Requested FTP SSL level failed */
    CURLE_SEND_FAIL_REWIND,        /* 65 - Sending the data requires a rewind
                                    that failed */
    CURLE_SSL_ENGINE_INITFAILED,   /* 66 - failed to initialise ENGINE */
    CURLE_LOGIN_DENIED,            /* 67 - user, password or similar was not
                                    accepted and we failed to login */
    CURLE_TFTP_NOTFOUND,           /* 68 - file not found on server */
    CURLE_TFTP_PERM,               /* 69 - permission problem on server */
    CURLE_REMOTE_DISK_FULL,        /* 70 - out of disk space on server */
    CURLE_TFTP_ILLEGAL,            /* 71 - Illegal TFTP operation */
    CURLE_TFTP_UNKNOWNID,          /* 72 - Unknown transfer ID */
    CURLE_REMOTE_FILE_EXISTS,      /* 73 - File already exists */
    CURLE_TFTP_NOSUCHUSER,         /* 74 - No such user */
    CURLE_CONV_FAILED,             /* 75 - conversion failed */
    CURLE_CONV_REQD,               /* 76 - caller must register conversion
                                    callbacks using curl_easy_setopt options
                                    CURLOPT_CONV_FROM_NETWORK_FUNCTION,
                                    CURLOPT_CONV_TO_NETWORK_FUNCTION, and
                                    CURLOPT_CONV_FROM_UTF8_FUNCTION */
    CURLE_SSL_CACERT_BADFILE,      /* 77 - could not load CACERT file, missing
                                    or wrong format */
    CURLE_REMOTE_FILE_NOT_FOUND,   /* 78 - remote file not found */
    CURLE_SSH,                     /* 79 - error from the SSH layer, somewhat
                                    generic so the error message will be of
                                    interest when this has happened */

    CURLE_SSL_SHUTDOWN_FAILED,     /* 80 - Failed to shut down the SSL
                                    connection */
    CURLE_AGAIN,                   /* 81 - socket is not ready for send/recv,
                                    wait till it's ready and try again (Added
                                    in 7.18.2) */
    CURLE_SSL_CRL_BADFILE,         /* 82 - could not load CRL file, missing or
                                    wrong format (Added in 7.19.0) */
    CURLE_SSL_ISSUER_ERROR,        /* 83 - Issuer check failed.  (Added in
                                    7.19.0) */
    CURL_LAST /* never use! */
};

#define CURLOPTTYPE_LONG          0
#define CURLOPTTYPE_OBJECTPOINT   10000
#define CURLOPTTYPE_FUNCTIONPOINT 20000
#define CURLOPTTYPE_OFF_T         30000


#define LONG          CURLOPTTYPE_LONG
#define OBJECTPOINT   CURLOPTTYPE_OBJECTPOINT
#define FUNCTIONPOINT CURLOPTTYPE_FUNCTIONPOINT
#define OFF_T         CURLOPTTYPE_OFF_T
/* TODO ifndef undef */
#define CINIT(%1,%2,%3) CURLOPT_%1 = %2 + %3

/* TODO remove unsupported options */
enum CURLoption {
    /* The full URL to get/put */
    CINIT(URL, OBJECTPOINT, 2),

    /* Port number to connect to, if other than default. */
    CINIT(PORT, LONG, 3),

    /* Name of proxy to use. */
    CINIT(PROXY, OBJECTPOINT, 4),

    /* "name:password" to use when fetching. */
    CINIT(USERPWD, OBJECTPOINT, 5),

    /* "name:password" to use with proxy. */
    CINIT(PROXYUSERPWD, OBJECTPOINT, 6),

    /* Time-out the read operation after this amount of seconds */
    CINIT(TIMEOUT, LONG, 13),

    /* POST static input fields. */
    CINIT(POSTFIELDS, OBJECTPOINT, 15),

    /* Set the referrer page (needed by some CGIs) */
    CINIT(REFERER, OBJECTPOINT, 16),

    /* Set the User-Agent string (examined by some CGIs) */
    CINIT(USERAGENT, OBJECTPOINT, 18),

    /* If the download receives less than "low speed limit" bytes/second
    * during "low speed time" seconds, the operations is aborted.
    * You could i.e if you have a pretty high speed connection, abort if
    * it is less than 2000 bytes/sec during 20 seconds.
    */

    /* Set the "low speed limit" */
    CINIT(LOW_SPEED_LIMIT, LONG, 19),

    /* Set the "low speed time" */
    CINIT(LOW_SPEED_TIME, LONG, 20),

    /* Set the continuation offset.
    *
    * Note there is also a _LARGE version of this key which uses
    * off_t types, allowing for large file offsets on platforms which
    * use larger-than-32-bit off_t's.  Look below for RESUME_FROM_LARGE.
    */
    CINIT(RESUME_FROM, LONG, 21),

    /* Set cookie in request: */
    CINIT(COOKIE, OBJECTPOINT, 22),

    /* This points to a linked list of headers, struct curl_slist kind */
    CINIT(HTTPHEADER, OBJECTPOINT, 23),

    /* This points to a linked list of post entries, struct curl_httppost */
    CINIT(HTTPPOST, OBJECTPOINT, 24),

    /* send TYPE parameter? */
    CINIT(CRLF, LONG, 27),

    /* send FILE * or void * to store headers to, if you use a callback it
     is simply passed to the callback unmodified */
    CINIT(WRITEHEADER, OBJECTPOINT, 29),

    /* point to a file to read the initial cookies from, also enables
     "cookie awareness" */
    CINIT(COOKIEFILE, OBJECTPOINT, 31),

    /* 35 = OBSOLETE */

    /* Custom request, for customizing the get command like
     HTTP: DELETE, TRACE and others
     FTP: to use a different list command
     */
    CINIT(CUSTOMREQUEST, OBJECTPOINT, 36),

    /* 38 is not used */

    CINIT(HEADER, LONG, 42),       /* throw the header out too */
    CINIT(NOBODY, LONG, 44),       /* use HEAD to get http document */
    CINIT(FAILONERROR, LONG, 45),  /* no output on http error codes >= 300 */
    CINIT(UPLOAD, LONG, 46),       /* this is an upload */
    CINIT(POST, LONG, 47),         /* HTTP POST method */

    CINIT(FOLLOWLOCATION, LONG, 52),  /* use Location: Luke! */

    CINIT(PUT, LONG, 54),          /* HTTP PUT */

    /* 55 = OBSOLETE */

    /* We want the referrer field set automatically when following locations */
    CINIT(AUTOREFERER, LONG, 58),

    /* Port of the proxy, can be set in the proxy string as well with:
     "[host]:[port]" */
    CINIT(PROXYPORT, LONG, 59),

    /* size of the POST input data, if strlen() is not good to use */
    CINIT(POSTFIELDSIZE, LONG, 60),

    /* tunnel non-http operations through a HTTP proxy */
    CINIT(HTTPPROXYTUNNEL, LONG, 61),

    /* Set the interface string to use as outgoing network interface */
    CINIT(INTERFACE, OBJECTPOINT, 62),

    /* 66 = OBSOLETE */
    /* 67 = OBSOLETE */

    /* Max amount of cached alive connections */
    CINIT(MAXCONNECTS, LONG, 71),

    /* 73 = OBSOLETE */

    /* Set to explicitly use a new connection for the upcoming transfer.
     Do not use this unless you're absolutely sure of this, as it makes the
     operation slower and is less friendly for the network. */
    CINIT(FRESH_CONNECT, LONG, 74),

    /* Set to explicitly forbid the upcoming transfer's connection to be re-used
     when done. Do not use this unless you're absolutely sure of this, as it
     makes the operation slower and is less friendly for the network. */
    CINIT(FORBID_REUSE, LONG, 75),

    /* Time-out connect operations after this amount of seconds, if connects
     are OK within this time, then fine... This only aborts the connect
     phase. [Only works on unix-style/SIGALRM operating systems] */
    CINIT(CONNECTTIMEOUT, LONG, 78),

    /* Set this to force the HTTP request to get back to GET. Only really usable
     if POST, PUT or a custom request have been used first.
    */
    CINIT(HTTPGET, LONG, 80),

    /* Specify which file name to write all known cookies in after completed
     operation. Set file name to "-" (dash) to make it go to stdout. */
    CINIT(COOKIEJAR, OBJECTPOINT, 82),

    /* Specify which HTTP version to use! This must be set to one of the
     CURL_HTTP_VERSION* enums set below. */
    CINIT(HTTP_VERSION, LONG, 84),

    /* Non-zero value means to use the global dns cache */
    CINIT(DNS_USE_GLOBAL_CACHE, LONG, 91), /* To become OBSOLETE soon */

    /* DNS cache timeout */
    CINIT(DNS_CACHE_TIMEOUT, LONG, 92),

    /* mark this as start of a cookie session */
    CINIT(COOKIESESSION, LONG, 96),

    /* Instruct libcurl to not use any signal/alarm handlers, even when using
     timeouts. This option is useful for multi-threaded applications.
     See libcurl-the-guide for more background information. */
    CINIT(NOSIGNAL, LONG, 99),

    /* Provide a CURLShare for mutexing non-ts data */
    CINIT(SHARE, OBJECTPOINT, 100),

    /* indicates type of proxy. accepted values are CURLPROXY_HTTP (default),
     CURLPROXY_SOCKS4, CURLPROXY_SOCKS4A and CURLPROXY_SOCKS5. */
    CINIT(PROXYTYPE, LONG, 101),

    /* Set the Accept-Encoding string. Use this to tell a server you would like
     the response to be compressed. */
    CINIT(ENCODING, OBJECTPOINT, 102),

    /* Set aliases for HTTP 200 in the HTTP Response header */
    CINIT(HTTP200ALIASES, OBJECTPOINT, 104),

    /* Continue to send authentication (user+password) when following locations,
     even when hostname changed. This can potentially send off the name
     and password to whatever host the server decides. */
    CINIT(UNRESTRICTED_AUTH, LONG, 105),

    /* Set this to a bitmask value to enable the particular authentications
     methods you like. Use this in combination with CURLOPT_USERPWD.
     Note that setting multiple bits may cause extra network round-trips. */
    CINIT(HTTPAUTH, LONG, 107),

    /* Set this to a bitmask value to enable the particular authentications
     methods you like. Use this in combination with CURLOPT_PROXYUSERPWD.
     Note that setting multiple bits may cause extra network round-trips. */
    CINIT(PROXYAUTH, LONG, 111),

    /* Same as TIMEOUT and CONNECTTIMEOUT, but with ms resolution */
    CINIT(TIMEOUT_MS, LONG, 155),
    CINIT(CONNECTTIMEOUT_MS, LONG, 156),

    /* POST volatile input fields. */
    CINIT(COPYPOSTFIELDS, OBJECTPOINT, 165),

    /* "name" and "pwd" to use when fetching. */
    CINIT(USERNAME, OBJECTPOINT, 173),
    CINIT(PASSWORD, OBJECTPOINT, 174),

    /* "name" and "pwd" to use with Proxy when fetching. */
    CINIT(PROXYUSERNAME, OBJECTPOINT, 175),
    CINIT(PROXYPASSWORD, OBJECTPOINT, 176),

    CURLOPT_LASTENTRY /* the last unused */
};

enum CURLFORMcode {
  CURL_FORMADD_OK, /* first, no error */

  CURL_FORMADD_MEMORY,
  CURL_FORMADD_OPTION_TWICE,
  CURL_FORMADD_NULL,
  CURL_FORMADD_UNKNOWN_OPTION,
  CURL_FORMADD_INCOMPLETE,
  CURL_FORMADD_ILLEGAL_ARRAY,
  CURL_FORMADD_DISABLED, /* libcurl was built with this disabled */

  CURL_FORMADD_LAST /* last */
};

#define CFINIT(%1) CURLFORM_%1

enum CURLformoption {
  CFINIT(NOTHING),        /********* the first one is unused ************/

  /*  */
  CFINIT(COPYNAME),
  CFINIT(PTRNAME),
  CFINIT(NAMELENGTH),
  CFINIT(COPYCONTENTS),
  CFINIT(PTRCONTENTS),
  CFINIT(CONTENTSLENGTH),
  CFINIT(FILECONTENT),
  CFINIT(ARRAY),
  CFINIT(OBSOLETE),
  CFINIT(FILE),

  CFINIT(BUFFER),
  CFINIT(BUFFERPTR),
  CFINIT(BUFFERLENGTH),

  CFINIT(CONTENTTYPE),
  CFINIT(CONTENTHEADER),
  CFINIT(FILENAME),
  CFINIT(END),
  CFINIT(OBSOLETE2),

  CFINIT(STREAM),

  CURLFORM_LASTENTRY /* the last unused */
};


/**
 * Initialize a cUrl session
 * @return Handle
 */
native Handle:curl_init();

/**
 * Close cUrl session
 * @param ch Curl Handle
 * @return True if success, false otherwise
 */
native curl_close(Handle:ch);

/**
 * Reset all options of a libcurl session handle
 * @param ch Curl Handle
 * @return True if success, false otherwise
 */
native curl_reset(Handle:ch);

/**
 * Return the last error number
 * @param ch Curl Handle
 * @return CURLcode
 */
native CURLcode:curl_errno(Handle:ch);

/**
 * Set an cell option for a cURL transfer
 * @param ch Curl Handle
 * @param option CURLoption (@see curl_is_cell_option)
 * @param value cell
 * @return CURLcode
 */
native CURLcode:curl_setopt_cell(Handle:ch, CURLoption:option, value);

/**
 * Set an string option for a cURL transfer
 * @param ch Curl Handle
 * @param option CURLoption (@see curl_is_string_option)
 * @param value String
 * @return CURLcode
 */
native CURLcode:curl_setopt_string(Handle:ch, CURLoption:option, const value[]);

/**
 * Set an handle option for a cURL transfer
 * @param Handle curl
 * @param CURLoption option
 * @param ch Curl Handle
 * @param option CURLoption (@see curl_is_string_option)
 * @param Handle value
 * @return CURLcode
 */
native CURLcode:curl_setopt_handle(Handle:ch, CURLoption:option, Handle:handle);


/**
 * Callback used by curl_exec, curl_thread_exec
 *
 * @param ch Curl Handle
 * @param code CURLcode (Result of performing)
 * @param response (String server response)
 * @param data (Data you passed)
 * public OnExecComplete(Handle:ch, CURLcode:code, const response[], any:data)
 */

/**
 * Perform a cURL session with blocking gameplay
 * @param ch Curl Handle
 * @param callback String (@see OnExecComplete)
 * @param value any data you passed
 * @return CURLcode
 */
native CURLcode:curl_exec(Handle:ch, const callback[] = "", any:value = 0);

/**
 * Perform a threaded cURL session without blocking gameplay
 * @param ch Curl Handle
 * @param callback String (@see OnExecComplete)
 * @param value any data you passed
 * @return True on success, otherwise False
 */
native CURLcode:curl_thread_exec(Handle:ch, const callback[] = "", any:value = 0);

/**
 * Copy a cURL handle along with all of its preferences
 * @param ch Curl Handle
 * @return Handle
 */
native Handle:curl_duphandle(Handle:curl);

/**
 * URL encodes the given string
 * @param ch Curl Handle
 * @param val given String
 * @param to String (Buffer to copy to)
 * @param length String buffer length
 * @return True on success, otherwise False
 */
native curl_escape(Handle:curl, const val[], to[], length);

/**
 * URL decodes the given string
 * @param ch Curl Handle
 * @param val given String
 * @param to String (Buffer to copy to)
 * @param length String buffer length
 * @return True on success, otherwise False
 */
native curl_unescape(Handle:curl, const val[], to[], length);

/**
 * Initialize curl_slist struct
 * @return Handle
 */
native Handle:curl_create_slist();

/**
 * Close curl_slist struct
 * @param slist Handle
 * @return True on success, otherwise False
 */
native curl_destroy_slist(Handle:slist);

/**
 * Appends string to slist struct
 * @param slist Handle
 * @param str String
 * @return True on success, otherwise False
 */
native curl_slist_append(Handle:slist, const str[]);

/**
 * Initialize CurlWebForm (pair of curl_httppost struct objects)
 * @return Handle
 */
native Handle:curl_create_form();

/**
 * Destroy CurlWebForm
 * @param form Handle
 * @return True on success, otherwise False
 */
native Handle:curl_destroy_form(Handle:form);

/**
 * Add a section to a multipart/formdata HTTP POST
 * @param form Handle
 * any...
 */
native CURLFORMcode:curl_form_add(Handle:form, any:...);
