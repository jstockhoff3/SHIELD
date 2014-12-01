<?php
include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");

$system=$_GET['systemName'];

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error')); 
$query = "SELECT * FROM System WHERE Name='$system';";
$sth = mysqli_query($con,$query);

if($sth){
	if($r = mysqli_fetch_assoc($sth)){
		$return = array(
			'Armed' => $r["Armed"],
			'AlarmActive' => $r["AlarmActive"],
			);
		shield_output($return,1,'None');
	}
	else{
		shield_output('None',0,'System Not Found');
	}
}
else{
	shield_output('None',0,'Query Error');
}

mysqli_close($con)

?>