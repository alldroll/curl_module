#include <amxmodx>
#include <curl>
#include <utest>

new testN = 0

#define TEST_INIT(%1)   server_print("[Testing %s]", %1);   testN = 0

#define TEST_OP(%1,%2,%3)  server_print("test%d %s", ++testN, (cell:%1%3cell:%2) ? "OK" : "FAIL")

#define TEST_EQUAL(%1,%2)  TEST_OP(%1,%2,==)

#define TEST_NEQUAL(%1,%2)  TEST_OP(%1,%2,!=)

/*
 * TODO use TestT
 * Add doc
 *

 * Bugs:
 * test number does not match actual number because of threaded performs (scope too)
 */

enum TestT
{
    TEST_PERFORM = 0,
    TEST_T_PERFORM,
    TEST_HTTPHEADER,
    TEST_POSTFIELDS,
    TEST_FORM
};

stock testPerformProvider[][] = {
    "Perform test 0",
    "Perform test 1"
}

stock testTPerformProvider[][] = {
    "Thread test 0",
    "Thread test 1"
}

public CURLcode:helperSetUrl(Handle:curl, testN, TestT:type)
{
    new url[120]
    format(url, 120, "http://localhost/?testEN=%d&type=%d", testN, type)
    return curl_setopt_string(curl, CURLOPT_URL, url)
}

public OnExecComplete(Handle:curl, CURLcode:code, const response[], any:testEN)
{
    /*
    TEST_NEQUAL(curl, INVALID_HANDLE)
    TEST_EQUAL(code, CURLE_OK)
    TEST_EQUAL(equal(testPerformProvider[testEN], response), true)
    */
}

public OnThreadExecComplete(Handle:curl, CURLcode:code, const response[], any:testEN)
{
    TEST_NEQUAL(curl, INVALID_HANDLE)
    TEST_EQUAL(code, CURLE_OK)
    TEST_EQUAL(equal(testTPerformProvider[testEN], response), true)
}

public OnCheckOkComplete(Handle:curl, CURLcode:code, const response[], any:testEN)
{
    TEST_EQUAL(code, CURLE_OK)
    TEST_EQUAL(equal("ok", response), true)
}

public OnCheckFormOkComplete(Handle:curl, CURLcode:code, const response[], any:form)
{
    TEST_EQUAL(code, CURLE_OK)
    TEST_EQUAL(equal("ok", response), true)
    TEST_EQUAL(curl_destroy_form(form), 1)
    TEST_EQUAL(curl_close(curl), 1)
}

TEST_LIST = {
    { "test1", "simple create/destroy curl handle" },
    { "test2", "setopt PORT URL" },
    { "test3", "curl simple exec" },
    { "test4", "create diplicate" },
    { "test5", "curl reset" },
    { "test6", "curl escape/unescape" },
    { "test7", "curl exec callback" },
    { "test8", "curl_slist create/destroy + append" },
    TEST_LIST_END
};

public run_test()
{
    UTEST_RUN(UT_VERBOSE)
}

START_TEST(test1) {
    new Handle:curl = curl_init()
    ASSERT_TRUE(INVALID_HANDLE != curl)
    ASSERT_TRUE(curl_close(curl))
} END_TEST

START_TEST(test2) {
    new Handle:curl = curl_init()
    ASSERT_TRUE(curl_setopt_cell(curl, CURLOPT_PORT, 8080) == CURLE_OK)
    ASSERT_TRUE(curl_setopt_string(curl, CURLOPT_URL, "localhost") == CURLE_OK)
    curl_close(curl)
} END_TEST

START_TEST(test3) {
    new Handle:curl = curl_init()
    curl_setopt_cell(curl, CURLOPT_PORT, 8080)
    curl_setopt_string(curl, CURLOPT_URL, "localhost")
    ASSERT_TRUE_MSG(curl_exec(curl) == CURLE_OK, "exec should return CURLE_OK")
    curl_close(curl)
} END_TEST

START_TEST(test4) {
    new Handle:curl = curl_init()
    curl_setopt_cell(curl, CURLOPT_PORT, 8080)
    curl_setopt_string(curl, CURLOPT_URL, "localhost")

    new Handle:dupl = curl_duphandle(curl);
    ASSERT_TRUE_MSG(dupl != INVALID_HANDLE, "duplicate has to be valid")

    new CURLcode:code = curl_exec(curl)
    curl_close(curl)
    ASSERT_TRUE(curl_exec(dupl) == code)
    ASSERT_INT_EQ(curl_close(dupl), 1)
} END_TEST

START_TEST(test5) {
    new Handle:curl = curl_init()
    curl_setopt_cell(curl, CURLOPT_PORT, 1010)

    ASSERT_INT_EQ(curl_reset(curl), 1)
    curl_setopt_string(curl, CURLOPT_URL, "localhost")
    ASSERT_TRUE(curl_exec(curl) == CURLE_OK)
    curl_close(curl)
} END_TEST

START_TEST(test6) {
    new Handle:curl = curl_init()

    new buf[20]
    curl_escape(curl, "Hello World", buf, 20)
    ASSERT_STRING_EQ(buf, "Hello%20World")

    curl_unescape(curl, buf, buf, 20)
    ASSERT_STRING_EQ(buf, "Hello World")

    curl_close(curl)
} END_TEST

START_TEST(test7) {
    new Handle:curl = curl_init()
    curl_setopt_cell(curl, CURLOPT_PORT, 8080)

    helperSetUrl(curl, 0, TEST_PERFORM)
    ASSERT_TRUE(curl_exec(curl, "OnExecComplete", 0))

    helperSetUrl(curl, 1, TEST_PERFORM)
    ASSERT_TRUE(curl_exec(curl, "OnExecComplete", 1))

    curl_close(curl)
} END_TEST

START_TEST(test8) {
    new Handle:slist = curl_create_slist();
    ASSERT_TRUE(slist != INVALID_HANDLE)

    ASSERT_INT_EQ(curl_slist_append(slist, "test_str"), 1)
    ASSERT_INT_EQ(curl_slist_append(slist, "test_str2"), 1)

    ASSERT_INT_EQ(curl_destroy_slist(slist), 1)
} END_TEST

START_TEST(test9) {
    SKIP_TEST("TODO")

#if false

    {
        TEST_INIT("curl thread exec callback")

        new Handle:curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)

        TEST_EQUAL(curl_setopt_cell(curl, CURLOPT_PORT, 8080), CURLE_OK)

        TEST_EQUAL(helperSetUrl(curl, 0, TEST_T_PERFORM), CURLE_OK)
        curl_thread_exec(curl, "OnThreadExecComplete", 0)

        TEST_EQUAL(helperSetUrl(curl, 1, TEST_T_PERFORM), CURLE_OK)
        curl_thread_exec(curl, "OnThreadExecComplete", 1)

        TEST_EQUAL(curl_close(curl), 1)
    }

    {
        TEST_INIT("curl add header opt")

        new Handle:chunk = curl_create_slist();
        TEST_NEQUAL(chunk, INVALID_HANDLE);
        TEST_EQUAL(curl_slist_append(chunk, "Cookie: ID=1234"), 1)

        new Handle:curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)

        TEST_EQUAL(curl_setopt_cell(curl, CURLOPT_PORT, 8080), CURLE_OK)

        TEST_EQUAL(helperSetUrl(curl, 0, TEST_HTTPHEADER), CURLE_OK)
        TEST_EQUAL(curl_setopt_handle(curl, CURLOPT_HTTPHEADER, chunk), CURLE_OK)

        curl_exec(curl, "OnCheckOkComplete")

        TEST_EQUAL(curl_close(curl), 1)

        /* WARNING! you have to destory slist after thread_exec complete */
        /* maybe create slist copy for threaded exec? */
        TEST_EQUAL(curl_destroy_slist(chunk), 1)
    }

    {
        TEST_INIT("curl post field test")

        new Handle:curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)

        TEST_EQUAL(curl_setopt_cell(curl, CURLOPT_PORT, 8080), CURLE_OK)
        TEST_EQUAL(helperSetUrl(curl, 0, TEST_POSTFIELDS), CURLE_OK)
        TEST_EQUAL(curl_setopt_string(curl, CURLOPT_POSTFIELDS, "postfield=postvalue"), CURLE_OK)

        curl_exec(curl, "OnCheckOkComplete")
        TEST_EQUAL(curl_close(curl), 1)
    }

    {
        TEST_INIT("curl form create/destroy")

        new Handle:form = curl_create_form()
        TEST_NEQUAL(form, INVALID_HANDLE)

        TEST_EQUAL(curl_destroy_form(form), 1)
    }

    {
        TEST_INIT("curl form add")
        new Handle:form = curl_create_form()

        TEST_NEQUAL(form, INVALID_HANDLE)

        new CURLFORMcode:code = curl_form_add(
            form,
            CURLFORM_COPYNAME,
            "htmlcode",
            CURLFORM_COPYCONTENTS,
            "<HTML></HTML>",
            CURLFORM_CONTENTTYPE,
            "text/html",
            CURLFORM_END
        )

        TEST_EQUAL(code, CURL_FORMADD_OK)

        TEST_EQUAL(curl_destroy_form(form), 1)
    }

    {
        TEST_INIT("curl form opt")
        new Handle:form = curl_create_form()

        TEST_NEQUAL(form, INVALID_HANDLE)

        new CURLFORMcode:code = curl_form_add(
            form,
            CURLFORM_COPYNAME,
            "submit",
            CURLFORM_COPYCONTENTS,
            "send",
            CURLFORM_END
        )

        TEST_EQUAL(code, CURL_FORMADD_OK)

        new Handle:curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)

        TEST_EQUAL(curl_setopt_cell(curl, CURLOPT_PORT, 8080), CURLE_OK)
        TEST_EQUAL(helperSetUrl(curl, 0, TEST_FORM), CURLE_OK)
        TEST_EQUAL(curl_setopt_handle(curl, CURLOPT_HTTPPOST, form), CURLE_OK)

        TEST_EQUAL(curl_exec(curl, "OnCheckOkComplete"), CURLE_OK)
        TEST_EQUAL(curl_close(curl), 1)

        TEST_EQUAL(curl_destroy_form(form), 1)
    }

    {
        TEST_INIT("curl thread form opt")
        new Handle:form = curl_create_form()

        TEST_NEQUAL(form, INVALID_HANDLE)

        new CURLFORMcode:code = curl_form_add(
            form,
            CURLFORM_COPYNAME,
            "submit",
            CURLFORM_COPYCONTENTS,
            "send",
            CURLFORM_END
        )

        TEST_EQUAL(code, CURL_FORMADD_OK)

        new Handle:curl = curl_init()
        TEST_NEQUAL(curl, INVALID_HANDLE)

        TEST_EQUAL(curl_setopt_cell(curl, CURLOPT_PORT, 8080), CURLE_OK)
        TEST_EQUAL(helperSetUrl(curl, 0, TEST_FORM), CURLE_OK)
        TEST_EQUAL(curl_setopt_handle(curl, CURLOPT_HTTPPOST, form), CURLE_OK)

        TEST_EQUAL(curl_thread_exec(curl, "OnCheckFormOkComplete", form), 1)
    }

    {
        TEST_INIT("curl form invalid")

        new Handle:form = curl_create_form()

        new CURLFORMcode:code = curl_form_add(
            form,
            CURLFORM_COPYNAME,
            "submit",
            CURLFORM_END
        )

        TEST_EQUAL(code, CURL_FORMADD_INCOMPLETE)

        TEST_EQUAL(curl_destroy_form(form), 1)
    }
#endif
} END_TEST

public plugin_init()
{
    register_plugin("curl_unit_test", "1.0", "alldroll")
    set_task(2.0, "run_test")
}
