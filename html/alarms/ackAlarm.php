<?php
include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");

$time=$_GET['time'];

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error')); 
$query = "SELECT * FROM AlarmHistory WHERE AlarmTime='$time';";
$sth = mysqli_query($con,$query);

if($time==null)
{
	shield_output('None',0,'Missing time input');
}
else{
	if($sth){ 
		if($r = mysqli_fetch_assoc($sth)){
			$query=
			"UPDATE AlarmHistory
			SET ACK=1
			WHERE AlarmTime='$time';";
			mysqli_query($con,$query);
			$systemName = $r['SystemName'];
			$query=
			"UPDATE System
			SET AlarmActive=0
			WHERE Name='$systemName';";
			mysqli_query($con,$query);
			shield_output('Newest alarm acknowledged, system alarm deactivated',1,'None');
		}
		else{
			shield_output('None',0,'Alarm Not Found');
		}
	}
	else{
		shield_output('None',0,'Query Error');
	}
}

mysqli_close($con);

?>