#include <amxmodx>
#include <curl>

#define ERROR   0
#define SUCCESS 1

new testN = 0

#define TEST_INIT(%1)   server_print("[Testing %s]", %1);   testN = 0

#define TEST_OP(%1,%2,%3)  server_print("test%d %s", ++testN, (cell:%1%3cell:%2) ? "OK" : "FAIL")

#define TEST_EQUAL(%1,%2)  TEST_OP(%1,%2,==)

#define TEST_NEQUAL(%1,%2)  TEST_OP(%1,%2,!=)

stock run_test()
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

}

public plugin_init()
{
    register_plugin("curl_unit_test", "1.0", "alldroll")
    run_test()
}
