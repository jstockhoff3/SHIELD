<?php

//get status of a specific motion sensor

include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");

$name=$_GET['poeName'];

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error')); 
$query = "SELECT * FROM PointsOfEntry WHERE EntryName='$name';";
$sth = mysqli_query($con,$query);

if($name==null){
	shield_output('None',0,'No poeName entered');
}
else{
	if($sth){
		if($r = mysqli_fetch_assoc($sth)){
			$return = array(
				'SystemName' => $r["SystemName"],
				'Locked' => $r["Locked"],
				'Open' => $r["Open"]
				);
			shield_output($return,1,'None');
		}
		else{
			shield_output('None',0,'POE Not Found');
		}
	}
	else{
		shield_output('None',0,'Query Error');
	}
}

mysqli_close($con)

?>