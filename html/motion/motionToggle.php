<?php
include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");

$name=$_GET['sens'];

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error'));
$query = "SELECT * FROM Motion WHERE Name='$name';";
$sth = mysqli_query($con,$query);

if($name==null){
	shield_output('None',0,'Missing sens');
}
else{
	if($sth){
		if($r = mysqli_fetch_assoc($sth)){
			$locked = $r['TriggerAlarm'];
			if($locked==1){
				$query=
				"UPDATE Motion
				SET TriggerAlarm=0
				WHERE Name='$name'";
				mysqli_query($con,$query);
				shield_output("$name trigger is now inactive",1,'None');
			}
			else if($locked==0){
				$query=
				"UPDATE Motion
				SET TriggerAlarm=1
				WHERE Name='$name'";
				mysqli_query($con,$query);
				shield_output("$name trigger is now active",1,'None');
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