#include <amxmodx>
#include <curl>
#include <utest>

#define BASE_URL "http://localhost/?testEN=%d&type=%d"

enum TestT
{
    TEST_PERFORM = 0,
    TEST_T_PERFORM,
    TEST_HTTPHEADER,
    TEST_POSTFIELDS,
    TEST_FORM
};

stock callbackOk = true

stock testPerformProvider[][] = {
    "Perform test 0",
    "Perform test 1"
}

stock testTPerformProvider[][] = {
    "Thread test 0",
    "Thread test 1"
}

TEST_LIST = {
    { "test1", "simple create/destroy curl handle" },
    { "test2", "try to setup PORT URL" },
    { "test3", "simple curl_exec" },
    { "test4", "try to create curl handle duplicate" },
    { "test5", "testing curl_reset" },
    { "test6", "testing escape/unescape string" },
    { "test7", "testing curl_exec with callback" },
    { "test8", "curl_slist create/destroy + append" },
    { "test9", "try to setup cookies" },
    { "test10", "try to pass post data"},
    { "test11", "curl_form create/destroy"},
    { "test12", "curl_form_add testing"},
    { "test13", "try to post curl_form"},
    { "test14", "create incomplete curl_form"},
    { "test15", "curl_thread_exec testing"},
    { "test16", "curl_thread_exec with curl_form"},
    TEST_LIST_END
};

public CURLcode:helperSetUrl(Handle:curl, testN, TestT:type)
{
    new url[120]
    format(url, 120, BASE_URL, testN, type)
    return curl_setopt_string(curl, CURLOPT_URL, url)
}

public OnExecComplete(Handle:curl, CURLcode:code, const response[], any:testEN)
{
    callbackOk = equal(testPerformProvider[testEN], response)
}

public OnThreadExecComplete(Handle:curl, CURLcode:code, const response[], any:testEN)
{
    /*threadCallbackOk = (curl != INVALID && equal(testTPerformProvider[testEN], response))*/
}

public OnCheckOkComplete(Handle:curl, CURLcode:code, const response[], any:testEN)
{
    callbackOk = equal("ok", response)
}

public OnCheckFormOkComplete(Handle:curl, CURLcode:code, const response[], any:form)
{
    callbackOk = equal("ok", response)
    curl_destroy_form(form)
    curl_close(curl)
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
    curl_setopt_string(curl, CURLOPT_URL, "http://google.com")
    ASSERT_TRUE(curl_exec(curl) == CURLE_OK)
    curl_close(curl)
} END_TEST

START_TEST(test6) {
    new Handle:curl = curl_init()

    new buf[20]
    curl_escape(curl, "Hello World", buf, sizeof(buf))
    ASSERT_STRING_EQ(buf, "Hello%20World")

    curl_unescape(curl, buf, buf, sizeof(buf))
    ASSERT_STRING_EQ(buf, "Hello World")

    curl_close(curl)
} END_TEST

START_TEST(test7) {
    new Handle:curl = curl_init()
    curl_setopt_cell(curl, CURLOPT_PORT, 8080)

    helperSetUrl(curl, 0, TEST_PERFORM)
    callbackOk = false
    ASSERT_INT_EQ(curl_exec(curl, "OnExecComplete", 0), CURLE_OK)
    ASSERT_TRUE(callbackOk)

    helperSetUrl(curl, 1, TEST_PERFORM)
    callbackOk = false
    ASSERT_INT_EQ(curl_exec(curl, "OnExecComplete", 1), CURLE_OK)
    ASSERT_TRUE(callbackOk)

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
    new Handle:chunk = curl_create_slist();
    ASSERT_INT_EQ(curl_slist_append(chunk, "Cookie: ID=1234"), 1)

    new Handle:curl = curl_init()

    curl_setopt_cell(curl, CURLOPT_PORT, 8080)
    helperSetUrl(curl, 0, TEST_HTTPHEADER)

    ASSERT_INT_EQ(curl_setopt_handle(curl, CURLOPT_HTTPHEADER, chunk), CURLE_OK)

    callbackOk = false
    ASSERT_INT_EQ(curl_exec(curl, "OnCheckOkComplete"), CURLE_OK)
    ASSERT_TRUE(callbackOk)

    curl_close(curl)
    /* WARNING! you have to destory slist after thread_exec complete */
    /* maybe create slist copy for threaded exec? */
    curl_destroy_slist(chunk)
} END_TEST

START_TEST(test10) {
    new Handle:curl = curl_init()

    curl_setopt_cell(curl, CURLOPT_PORT, 8080)
    helperSetUrl(curl, 0, TEST_POSTFIELDS)
    ASSERT_INT_EQ(curl_setopt_string(curl, CURLOPT_POSTFIELDS, "postfield=postvalue"), CURLE_OK)

    callbackOk = false
    ASSERT_INT_EQ(curl_exec(curl, "OnCheckOkComplete"), CURLE_OK)
    curl_close(curl)
} END_TEST

START_TEST(test11) {
    new Handle:form = curl_create_form()
    ASSERT_INT_NEQ(form, INVALID_HANDLE)
    ASSERT_INT_EQ(curl_destroy_form(form), 1)
} END_TEST

START_TEST(test12) {
    new Handle:form = curl_create_form()

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

    ASSERT_INT_EQ(code, CURL_FORMADD_OK)
    ASSERT_INT_EQ(curl_destroy_form(form), 1)
} END_TEST

START_TEST(test13) {
    new Handle:form = curl_create_form()
    new Handle:curl = curl_init()

    new CURLFORMcode:code = curl_form_add(
        form,
        CURLFORM_COPYNAME,
        "submit",
        CURLFORM_COPYCONTENTS,
        "send",
        CURLFORM_END
    )

    ASSERT_INT_EQ(code, CURL_FORMADD_OK)

    curl_setopt_cell(curl, CURLOPT_PORT, 8080)
    helperSetUrl(curl, 0, TEST_FORM)
    ASSERT_INT_EQ(curl_setopt_handle(curl, CURLOPT_HTTPPOST, form), CURLE_OK)

    callbackOk = false
    ASSERT_INT_EQ(curl_exec(curl, "OnCheckOkComplete"), CURLE_OK)
    ASSERT_TRUE(callbackOk)
    curl_close(curl)

    ASSERT_INT_EQ(curl_destroy_form(form), 1)
} END_TEST

START_TEST(test14) {
    new Handle:form = curl_create_form()
    new CURLFORMcode:code = curl_form_add(
        form,
        CURLFORM_COPYNAME,
        "submit",
        CURLFORM_END
    )

    ASSERT_INT_EQ(code, CURL_FORMADD_INCOMPLETE)
    ASSERT_INT_EQ(curl_destroy_form(form), 1)
} END_TEST

/* TODO check callback result */
START_TEST(test15) {
    new Handle:curl = curl_init()

    curl_setopt_cell(curl, CURLOPT_PORT, 8080)

    helperSetUrl(curl, 0, TEST_T_PERFORM)
    ASSERT_TRUE(curl_thread_exec(curl, "OnThreadExecComplete", 0))

    helperSetUrl(curl, 1, TEST_T_PERFORM)
    ASSERT_TRUE(curl_thread_exec(curl, "OnThreadExecComplete", 1))

    ASSERT_INT_EQ(curl_close(curl), 1)
} END_TEST

/* TODO check callback result */
START_TEST(test16) {
    new Handle:form = curl_create_form()
    curl_form_add(
        form,
        CURLFORM_COPYNAME,
        "submit",
        CURLFORM_COPYCONTENTS,
        "send",
        CURLFORM_END
    )
    new Handle:curl = curl_init()

    curl_setopt_cell(curl, CURLOPT_PORT, 8080)
    helperSetUrl(curl, 0, TEST_FORM)
    curl_setopt_handle(curl, CURLOPT_HTTPPOST, form)

    ASSERT_INT_EQ(curl_thread_exec(curl, "OnCheckFormOkComplete", form), 1)
} END_TEST

public run_test()
{
    UTEST_RUN(UT_VERBOSE)
}

public plugin_init()
{
    register_plugin("curl_unit_test", "1.1", "alldroll")
    set_task(2.0, "run_test")
}
