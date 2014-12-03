<?php

//triggerStatus 1: trigger alarm if motion found
//				0: no action if motion found

include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");

$sensor=$_GET['sensorName'];
$triggerStatus=$_GET['triggerStatus'];
$sysname=$_GET['systemName'];


$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error'));
$res = mysqli_query($con,"SELECT * FROM Motion WHERE Name='$sensor'");
	
if($sensor==null or $sysname==null){
	shield_output('None',0,'Missing Inputs');
}
else{
	if($res){
		if(mysqli_num_rows($res) == 0){
			if($triggerStatus==null){
				$query=
				"INSERT INTO Motion (Name,SystemName) VALUES ('$sensor','$sysname');";
				mysqli_query($con,$query);
				shield_output("Entry $sensor Created",1,'None');
			}
			else{
				$query=
				"INSERT INTO Motion (Name,SystemName,TriggerAlarm) VALUES ('$sensor','$sysname','$triggerStatus');";
				mysqli_query($con,$query);
				shield_output("Entry $sensor Created",1,'None');
			}
		}
		else{
			shield_output('None',0,'Duplicate Sensor Found');
		}
	}
	else{
		shield_output('None',0,'Initial Query Error');
	}
}

mysqli_close($con);

?>