<?php
    if($argc != 4 || !is_numeric($num=$argv[3]) || $num<=0) {
        die("arguments error:usage php ".$argv[0]." inputfile outputfile_prefix filenum");
    }
    $lines=file($argv[1]);
    $outputfiles_prefix = $argv[2];
    $outputfiles=array();
    if($num>100) $num=100;
    for($i=0;$i<$num;$i++) {
        $outputfilenames=sprintf("%s%02d",$outputfiles_prefix,$i);
        $outputfiles[] = fopen($outputfilenames,'w');
    }
    $i=0;
    foreach($lines as $line) {
        fprintf($outputfiles[$i%$num],"%s",$line);
        $i++;
    }
    for($i=0;$i<$num;$i++) {
        fclose($outputfiles[$i]);
    }
?>
