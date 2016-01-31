#include <amxmodx>
#include <curl>

#define ERROR   0
#define SUCCESS 1

new testN = 0

#define TEST_INIT(%1)   server_print("[Testing %s]", %1);   testN = 0

#define TEST_ASSERT(%1,%2)  server_print("test%d %s", ++testN, cell:%1==cell:%2 ? "OK" : "FAIL")

#define TEST_ASSERT_SUCCESS(%1) TEST_ASSERT(%1, SUCCESS)

#define TEST_ASSERT_ERROR(%1)   TEST_ASSERT(%1, ERROR)

public plugin_modules()
{
        require_module("curl")
}

stock run_test()
{
    {
        TEST_INIT("curl create/destroy")

        new Handle:curl = curl_init()
        TEST_ASSERT_SUCCESS(curl_close(curl))
    }
}

public plugin_init()
{
    register_plugin("curl_unit_test", "1.0", "alldroll")
    run_test()
}
