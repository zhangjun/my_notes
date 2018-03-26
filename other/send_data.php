<?php

$str  = '{"log_type":"mobile_query","status":1000,"date":"2015-02-1113:00:01","uid":"123456789","search":"youhu","time":0.20535993576}';
$sock = fsockopen('udp://172.16.228.200', 16385, $errno, $errstr, 1);
fwrite($sock, $str);
fclose($sock);
var_dump($errstr);


?>
