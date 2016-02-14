<?php

$responses = [
    'Hello World',
    'How are you?'
];

if (isset($_REQUEST['testEN']) && isset($responses[$_REQUEST['testEN']])) {
    echo $responses[$_REQUEST['testEN']];
} else {
    echo "Good bye";
}

