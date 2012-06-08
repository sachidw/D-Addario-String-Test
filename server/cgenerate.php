<html>
<head>
<title>Test Config Generation</title>
</head>
<body>
<?php

$f0 = $_GET["f0"];
$f1 = $_GET["f1"];
$f2 = $_GET["f2"];
$f3 = $_GET["f3"];
$f4 = $_GET["f4"];
$f5 = $_GET["f5"];

$t0 = $_GET["t0"];
$t1 = $_GET["t1"];
$t2 = $_GET["t2"];
$t3 = $_GET["t3"];
$t4 = $_GET["t4"];
$t5 = $_GET["t5"];

$type0 = $_GET["type0"]; 
$type1 = $_GET["type1"]; 
$type2 = $_GET["type2"]; 
$type3 = $_GET["type3"]; 
$type4 = $_GET["type4"]; 
$type5 = $_GET["type5"]; 

$brand0 = $_GET["brand0"];
$brand1 = $_GET["brand1"];
$brand2 = $_GET["brand2"];
$brand3 = $_GET["brand3"];
$brand4 = $_GET["brand4"];
$brand5 = $_GET["brand5"];

$length = $_GET["length"];
$interval = $_GET["interval"];

print("<b>Test Configuration File Generated!</b><br />Return to main page by <a href=index.html>Clicking Here</a></br></br>
To view config file <a href=testconfig.txt>Clicking Here</a></br></br>
");
$out = fopen("testconfig.txt", "w");
if (!$out) {
print("Could not generate file");
exit;
}
fputs ($out,implode,("\n"));
fwrite($out,"$f0\n$f1\n$f2\n$f3\n$f4\n$f5\n");
fwrite($out,"$t0\n$t1\n$t2\n$t3\n$t4\n$t5\n");
fwrite($out,"$interval\n");
fwrite($out,"$length\n");

fwrite($out,"$type0\n$type1\n$type2\n$type3\n$type4\n$type5\n");
fwrite($out,"$brand0\n$brand1\n$brand2\n$brand3\n$brand4\n$brand5\n");

fclose($out);
?>
</body>
</html> 
