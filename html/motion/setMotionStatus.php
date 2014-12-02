<?php

//triggerStatus 1: trigger alarm if motion found
//				0: no action if motion found
//motionFound 1: trip alarm if triggerStatus:1
//			  0: trip alarm if triggerStatus:0

include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");

$sensor=$_GET['sensorName'];
$motionStatus=$_GET['motionFound'];


$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error'));
$query = "SELECT * FROM Motion WHERE Name='$sensor';";
$sth = mysqli_query($con,$query);

if($sensor==null or $motionStatus==null){
	shield_output('None',0,'Missing Inputs');
}
else{
	if($sth){
		if($r = mysqli_fetch_assoc($sth)){
			$systemName = $r['SystemName'];
			if($motionStatus==1){
				$query=
				"UPDATE Motion
				SET MotionFound=1
				TimeFound=NOW()
				WHERE Name='$sensor'";
			}
			else if($motionStatus==0){
				$query=
				"UPDATE Motion
				SET MotionFound=0
				WHERE Name='$sensor'";
			}
			$sth = mysqli_query($con,$query);
			if($sth){
				$res = motionAlarmTrigger($con,$sensor);
				if($res){
					shield_output("Motion found and alarm triggered for System: $systemName",1,'None');
				}
				else{
					shield_output("Update successful, no alarm triggered.",1,'None');
				}
			}
			else{
				shield_output('None',0,'SQL Update Query Error');
			}
		}
		else{
			shield_output('None',0,'Sensor Not Found');
		}
	}
	else{
		shield_output('None',0,'Initial Query Error');
	}
}

mysqli_close($con)

?>