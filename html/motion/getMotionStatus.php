<?php

//get status of a specific motion sensor


include(dirname(__FILE__)."/../shield_DB_info.php");
include(dirname(__FILE__)."/../shield_PHP_lib.php");

$sensor=$_GET['sensorName'];

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error')); 
$query = "SELECT * FROM Motion WHERE Name='$sensor';";
$sth = mysqli_query($con,$query);

if($sensor==null){
	shield_output('None',0,'No sensorName entered');
}
else{
	if($sth){
		if($r = mysqli_fetch_assoc($sth)){
			$return = array(
				'SystemName' => $r["SystemName"],
				'MotionFound' => $r["MotionFound"],
				'TriggerAlarm' => $r["TriggerAlarm"],
				'TimeFound' => $r["TimeFound"]
				);
			shield_output($return,1,'None');
		}
		else{
			shield_output('None',0,'Sensor Not Found');
		}
	}
	else{
		shield_output('None',0,'Query Error');
	}
}

mysqli_close($con)

?>