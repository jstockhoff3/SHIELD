<?php
include 'shield_DB_info.php';

$sensor=$_GET['sensorName'];

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die("Error " . mysqli_error($con)); 
$query = "SELECT * FROM Motion WHERE Name='$sensor';";
$sth = mysqli_query($con,$query) OR die("Invalid query: $query". mysql_error());


if($r = mysqli_fetch_assoc($sth)){
	$return = array(
		'SystemName' => $r["SystemName"],
		'MotionFound' => $r["MotionFound"],
		'TriggerAlarm' => $r["TriggerAlarm"],
		'TimeFound' => $r["TimeFound"]
		);
	print json_encode($return);
}
else{
	print json_encode('Sensor not found');
}

mysqli_close($con)

?>