#if defined _utest_included
    #endinput
#endif
#define _utest_included

#define TEST_NAME_MAX_LEN 64
#define TEST_FUNC_MAX_LEN 66

/* TODO */
#define STOP_AFTER_FIRST_FAIL 0x1
#define VERBOSE 0x2

/**
 *
 */
enum _:_ScopeT {
    name[TEST_FUNC_MAX_LEN],
    total,
    options
}

/**
 *
 */
stock __scope[_ScopeT] = {"", 0, STOP_AFTER_FIRST_FAIL | VERBOSE}

/**
 *
 */
enum _:_TestT {
    name[TEST_NAME_MAX_LEN],
    func[TEST_FUNC_MAX_LEN],
    desc[200],
    assert_number
};

/**
 */
stock __test_list[20][_TestT]


/**
 *
 */
#define START_TEST(%1)\
public __%1(test[_TestT]) {\
    server_print("START TESTING %s", test[desc]);

/**
 *
 */
#define END_TEST }

/**
 *
 */
stock add_test(const test_name[], const test_desc[])
{
    new i = __scope[total]++

    if (i >= 20) {
        /* TODO log error*/
    }

    copy(__test_list[i][name], TEST_NAME_MAX_LEN, test_name)
    format(__test_list[i][func], TEST_FUNC_MAX_LEN, "__%s", test_name)
    copy(__test_list[i][desc], 200, test_desc)
}

/**
 *
 */
stock utest_run()
{
    for (new i = 0; i < __scope[total]; ++i) {
        if (callfunc_begin(__test_list[i][func])) {
            callfunc_push_array(_:__test_list[i], _TestT)
            callfunc_end()
        }
    }
}

#define ASSERT_TRUE(%1) test_assert_true(test, any:%1, __LINE__/*, "%s", #%1*/)

stock test_assert_true(test[_TestT], cond, line/*, const fmt[], any:...*/)
{
    new number = ++test[assert_number]

    if (cond) {
        server_print("[%s] OK #%d LINE %d", test[name], number, line)
    } else {
        server_print("[%s] FAIL #%d LINE %d", test[name], number, line)
    }

}

#undef TEST_FUNC_MAX_LEN
#undef TEST_NAME_MAX_LEN
