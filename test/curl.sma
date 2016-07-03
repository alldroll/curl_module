#include <amxmodx>
#include <curl>

new testN = 0

#define TEST_INIT(%1)   server_print("[Testing %s]", %1);   testN = 0

#define TEST_OP(%1,%2,%3)  server_print("test%d %s", ++testN, (cell:%1%3cell:%2) ? "OK" : "FAIL")

#define TEST_EQUAL(%1,%2)  TEST_OP(%1,%2,==)

#define TEST_NEQUAL(%1,%2)  TEST_OP(%1,%2,!=)

stock testResponseExpectedProvider[][] = {
    "Hello World",
    "How are you?",
    "TODO test header"
}

public OnExecComplete(Handle:curl, CURLcode:code, const response[], any:testEN)
{
    TEST_NEQUAL(curl, INVALID_HANDLE)
    TEST_EQUAL(code, CURLE_OK)
    TEST_EQUAL(equal(testResponseExpectedProvider[testEN], response), true)
}

public OnThreadExecComplete(Handle:curl, CURLcode:code, const response[], any:testEN)
{
    TEST_NEQUAL(curl, INVALID_HANDLE)
    TEST_EQUAL(code, CURLE_OK)
    TEST_EQUAL(equal(testResponseExpectedProvider[testEN], response), true)
}

/**
 * TODO add description
 *
 */
public run_test()
{
    {
        TEST_INIT("curl create/destroy")

        new Handle:curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)
        TEST_EQUAL(curl_close(curl), 1)
    }

    {
        TEST_INIT("curl setopt port, url")

        new Handle:curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)
        TEST_EQUAL(curl_setopt_cell(curl, CURLOPT_PORT, 8080), CURLE_OK)
        TEST_EQUAL(curl_setopt_string(curl, CURLOPT_URL, "localhost"), CURLE_OK)
        TEST_EQUAL(curl_close(curl), 1)
    }

    {
        TEST_INIT("curl test simple exec")

        new Handle:curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)
        TEST_EQUAL(curl_setopt_cell(curl, CURLOPT_PORT, 8080), CURLE_OK)
        TEST_EQUAL(curl_setopt_string(curl, CURLOPT_URL, "localhost"), CURLE_OK)
        TEST_EQUAL(curl_exec(curl), CURLE_OK)
        TEST_EQUAL(curl_close(curl), 1)
    }

    {
        TEST_INIT("curl test duplicate")

        new Handle:curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)
        TEST_EQUAL(curl_setopt_cell(curl, CURLOPT_PORT, 8080), CURLE_OK)
        TEST_EQUAL(curl_setopt_string(curl, CURLOPT_URL, "localhost"), CURLE_OK)

        new Handle:dupl = curl_duphandle(curl);
        TEST_NEQUAL(dupl, INVALID_HANDLE)

        TEST_EQUAL(curl_exec(dupl), CURLE_OK)
        TEST_EQUAL(curl_close(curl), 1)
        TEST_EQUAL(curl_exec(dupl), CURLE_OK)
        TEST_EQUAL(curl_close(dupl), 1)
    }

    {
        TEST_INIT("curl test reset")

        new Handle:curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)
        TEST_EQUAL(curl_setopt_cell(curl, CURLOPT_PORT, 1010), CURLE_OK)

        TEST_EQUAL(curl_reset(curl), 1);
        TEST_EQUAL(curl_setopt_string(curl, CURLOPT_URL, "localhost"), CURLE_OK)
        TEST_EQUAL(curl_exec(curl), CURLE_OK)
        TEST_EQUAL(curl_close(curl), 1)

        curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)
        TEST_EQUAL(curl_setopt_cell(curl, CURLOPT_PORT, 1010), CURLE_OK)
        TEST_EQUAL(curl_reset(curl), 1);

        new Handle:dupl = curl_duphandle(curl)
        TEST_NEQUAL(dupl, INVALID_HANDLE)
        TEST_EQUAL(curl_setopt_string(dupl, CURLOPT_URL, "http://google.com"), CURLE_OK)

        TEST_EQUAL(curl_close(curl), 1)
        TEST_EQUAL(curl_exec(dupl), CURLE_OK)
        TEST_EQUAL(curl_close(dupl), 1)
    }

    {
        TEST_INIT("curl escape/unescape")

        new Handle:curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)

        new buf[20]
        curl_escape(curl, "Hello World", buf, 20)
        TEST_EQUAL(equal(buf, "Hello%20World"), true)

        curl_unescape(curl, buf, buf, 20)
        TEST_EQUAL(equal(buf, "Hello World"), true)

        TEST_EQUAL(curl_close(curl), 1)
    }

    {
        TEST_INIT("curl exec callback")

        new Handle:curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)

        TEST_EQUAL(curl_setopt_cell(curl, CURLOPT_PORT, 8080), CURLE_OK)

        TEST_EQUAL(curl_setopt_string(curl, CURLOPT_URL, "http://localhost/?testEN=0"), CURLE_OK)
        curl_exec(curl, "OnExecComplete", 0)

        TEST_EQUAL(curl_setopt_string(curl, CURLOPT_URL, "http://localhost/?testEN=1"), CURLE_OK)
        curl_exec(curl, "OnExecComplete", 1)

        TEST_EQUAL(curl_close(curl), 1)
    }

    {
        TEST_INIT("curl thread exec callback")

        new Handle:curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)

        TEST_EQUAL(curl_setopt_cell(curl, CURLOPT_PORT, 8080), CURLE_OK)

        TEST_EQUAL(curl_setopt_string(curl, CURLOPT_URL, "http://localhost/?testEN=0"), CURLE_OK)
        curl_thread_exec(curl, "OnThreadExecComplete", 0)

        TEST_EQUAL(curl_setopt_string(curl, CURLOPT_URL, "http://localhost/?testEN=1"), CURLE_OK)
        curl_thread_exec(curl, "OnThreadExecComplete", 1)

        TEST_EQUAL(curl_close(curl), 1)
    }

    {
        TEST_INIT("curl_slist test create/destroy")

        new Handle:slist = curl_create_slist();
        TEST_NEQUAL(slist, INVALID_HANDLE);

        TEST_EQUAL(curl_slist_append(slist, "test_str"), 1)
        TEST_EQUAL(curl_slist_append(slist, "test_str2"), 1)

        TEST_EQUAL(curl_destroy_slist(slist), 1)
    }

    {
        TEST_INIT("curl add header opt")

        new Handle:chunk = curl_create_slist();
        TEST_NEQUAL(chunk, INVALID_HANDLE);
        TEST_EQUAL(curl_slist_append(chunk, "Referer: http://www.example.com/index.php"), 1)

        new Handle:curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)

        TEST_EQUAL(curl_setopt_cell(curl, CURLOPT_PORT, 8080), CURLE_OK)

        TEST_EQUAL(curl_setopt_string(curl, CURLOPT_URL, "http://localhost/?testEN=2"), CURLE_OK)
        curl_exec(curl, "OnExecComplete", 2)
    }
}

public plugin_init()
{
    register_plugin("curl_unit_test", "1.0", "alldroll")
    register_srvcmd("curltest", "run_test");
}
