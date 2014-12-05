<?php

//take image path as input
//creates entry in AllVisitors, by default new visitor is:
//		not known Known=0
//		not acknowledged ACK=0
//      TimeVisited = time of execution of this script

include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");

$image=$_GET['image'];


$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error'));
$res = mysqli_query($con,"SELECT * FROM AllVisitors");

if($image==null){
	shield_output('None',0,'No image path entered');
}
else{	
	if($res){
		$query=
		"INSERT INTO AllVisitors (TimeVisited,Image) VALUES (NOW(),'$image');";
		mysqli_query($con,$query);
		shield_output("Visitor created",1,'None');
	}
	else{
		shield_output('None',0,'Query Error');
	}
}


mysqli_close($con);

?>