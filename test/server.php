<?php

$responses = [
    'Hello World',
    'How are you?',
    'Header test'
];

ob_start();
var_dump($_REQUEST);
var_dump($_SERVER);
error_log(ob_get_clean(), 4);

if (isset($_REQUEST['testEN']) && isset($responses[$_REQUEST['testEN']])) {
    echo $responses[$_REQUEST['testEN']];
} else {
    echo "Good bye";
}

