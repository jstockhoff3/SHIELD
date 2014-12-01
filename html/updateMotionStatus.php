<?php
include 'shield_DB_info.php';

$sensor=$_GET['sensorName'];
$motionStatus=$_GET['motionFound'];
$triggerStatus=$_GET['triggerStatus'];


$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die("Error " . mysqli_error($con)); 
$query = "SELECT * FROM Motion WHERE Name='$sensor';";
$sth = mysqli_query($con,$query) OR die("Invalid query: $query". mysql_error());


if($r = mysqli_fetch_assoc($sth)){
	$query=
	"UPDATE Motion
	SET MotionFound='$motionStatus',
	TriggerAlarm='$triggerStatus'
	WHERE Name='$sensor'";
	mysqli_query($con,$query) OR die("Invalid query: $query". mysql_error());
	return json_encode('Sensor Updated');
}
else{
	print json_encode('Sensor not found');
}

mysqli_close($con)

?>