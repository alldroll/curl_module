Curl amxmodx module
===================

This module adds support to use some curl library functionality in pawn lang.

This module was not tested on windows, mac machines, on old amxmodx versions. So
you can help with it:). Requires libcurl >= 7.35.0.

### How does it works:

```pawn

#include <curl>

enum EventT
{
    DEATH = 0
};

public OnExecComplete(Handle:curl, CURLcode:code, const response[], any:eventType)
{
    /* some processing here */
    curl_close(curl)
}

public hook_death()
{
    /* take killerName, victinName, weapon  */

    new CURLcode:code = CURLE_OK
    new data[120]
    format(
        data,
        120,
        "{\"killer\": \"%s\", \"victim\": "%s", \"weapon\": \"%s\"}",
        killerName,
        victimName,
        weapon
    )

    new Handle:curl = curl_init()
    code = curl_setopt_string(curl, CURLOPT_URL, "http://localhost/")
    if (code != CURLE_OK) {
        /* TODO report error */
    }

    code = curl_setopt_string(curl, CURLOPT_POSTFIELDS, data)
    if (code != CURLE_OK) {
        /* TODO report error */
    }

    /* perform request without blocking main process */
    code = curl_thread_exec(curl, "OnExecComplete", DEATH)
    /* you can destroy curl handle here or in callback */
}

```
p.s. for more information see tests

### Notice

* functions curl_form_add, curl_destroy_form, curl_slist_append, curl_destroy_slist
are not thread safe now, so please, if you share curl_slist, curl_form handles
between threaded performs, make sure, that you use this functions after all performs
* there are wrong test number + test name for some test cases because of thread_exec

### Natives

```pawn
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
```
