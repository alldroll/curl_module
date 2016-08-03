<?php

define('VERBOSE', true);

class TestT
{
    const PERFORM = 0;
    const T_PERFORM = 1;
    const HTTP_HEADER = 2;
    const POSTFIELDS = 3;
    const HTTP_POST = 4;
}

$type = isset($_REQUEST['type']) ? $_REQUEST['type'] : TestT::PERFORM;
$testN = isset($_REQUEST['testEN']) ? $_REQUEST['testEN'] : 0;

switch ($type) {
    case TestT::PERFORM:
        $responses = [
            'Perform test 0',
            'Perform test 1'
        ];
        echo $responses[$testN];

        break;

    case TestT::T_PERFORM:
        $responses = [
            'Thread test 0',
            'Thread test 1'
        ];
        echo $responses[$testN];

        break;
    case TestT::HTTP_HEADER:
        $result = testHTTPHeader($testN);
        if ($result) {
            echo 'ok';
        } else {
            echo 'bad';
        }

        break;

    case TestT::POSTFIELDS:
        $success = isset($_POST['postfield']) && $_POST['postfield'] === 'postvalue';
        if ($success) {
            echo 'ok';
        } else {
            echo 'bad';
        }

        break;

    case TestT::HTTP_POST:
        $success = isset($_POST['submit']) && $_POST['submit'] === 'send';
        if ($success) {
            echo 'ok';
        } else {
            echo 'bad';
        }

        break;

    default: break;
}

/**
 * @param int $testN
 * @return bool
 */
function testHTTPHeader($testN)
{
    $success = false;
    switch ($testN) {
        case 0:
            $success = isset($_COOKIE['ID']) && $_COOKIE['ID'] === '1234';
            break;
        default: break;
    }

    return $success;
}

if (defined('VERBOSE') && VERBOSE) {
    ob_start();
    var_dump($_REQUEST);
    var_dump($_SERVER);
    error_log(ob_get_clean(), 4);
}
