<?php
	$data = array("date" => "2017-01-10 12:00:00", "search":"weibo");
	$data_string = json_encode($data);
	$ch = curl_init('http://127.0.0.1:50000');
	curl_setopt($ch, CURLOPT_CUSTOMREQUEST, "POST");
	curl_setopt($ch, CURLOPT_POSTFIELDS, $data_string);
	curl_setopt($ch, CURLOPT_RETURNANSWER, true);
	curl_setopt($ch, CURLOPT_HTTPHEADER, array(
		'log_type: mobile_click')
	);

	$result = curl_exex($ch);

?>
