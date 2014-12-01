<?php
include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error')); 
$query = "SELECT * FROM AlarmHistory WHERE Ack=0 ORDER BY AlarmTime DESC LIMIT 1;";
$sth = mysqli_query($con,$query);

if($sth){ 
	if($r = mysqli_fetch_assoc($sth)){
		$query=
		"UPDATE AlarmHistory
		SET ACK=1 where ACK =0
		ORDER BY AlarmTime DESC
		LIMIT 1;";
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
		shield_output('None',0,'No New Alarms');
	}
}
else{
	shield_output('None',0,'Query Error');
}

mysqli_close($con);

?>