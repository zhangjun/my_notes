<?php
$index_file = $argv[1];
$all_file = $argv[2];
$out_file = $argv[3];
$opts = array(   
  'http'=>array(   
    'method'=>"GET",   
    'timeout'=>3,//s
   )
);
$cnt=0;
while($cnt<2 &&($data_index = file_get_contents($index_file,false, stream_context_create($opts))) === False)
    $cnt++;
$data_index = file_get_contents($index_file);
$data_all = file_get_contents($all_file);

$index_array = explode("\n", $data_index);

$result = array();

foreach ($index_array as $line)
{
	if ($line == "")
		continue;
	$line_array = explode("\t", $line);
    if(count($line_array) != 2)
        $line_array = explode(" ",preg_replace("/\s(?=\s)/","\\1",$line));
	$page_url = $line_array[0];
	$type = $line_array[1];
	parseIndexPage($result, $page_url, $type);
}
#print_r($result);


$fp_out = fopen($out_file, "w");
#fprintf($fp_out, "%s\n", $data_all);
print_r($data_all);
foreach ($result as $item)
{
	fprintf($fp_out, "%s\t%s\n", $item['loc'], $item['type']);
}

function parseIndexPage(&$result, $page_url, $type)
{
	#$page_url = "http://api.tudou.com/v3/gw?method=special.album.index&appKey=a9edcbb9e147faa8&format=xml&partner=sogouv&category=cartoon";
	@$data = file_get_contents($page_url);
	@$xml = simplexml_load_string($data);
    if(empty($xml))
        return;
	$items = array();


	if (isset($xml->sitemap))
		$items = $xml->sitemap;
	if (isset($xml->sdd))
		$items = $xml->sdd;
	if (isset($xml->url))
		$items = $xml->url;
	$idx = 0;
	foreach ($items as $item)
	{
		foreach ($item as $key=>$value)
		{
			if ($key == "loc")
			{
				$new_data = array();
				$new_data['loc'] = (string)$value;
				$new_data['loc'] = str_replace("\n", "", $new_data['loc']);
				$new_data['type'] = $type;
				$result[] = $new_data;
				$idx++;
			}
		}
	}
	#print_log($page_url." has ".$idx." detail pages");
}

function print_log($msg)
{
    fprintf(STDERR, "[%s] [%s]\n", date("Y-m-d H:i:s"), $msg);
}

?>
