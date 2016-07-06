Curl amxmodx module
===================

This module adds support to use some curl library functionality in pawn lang.

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
