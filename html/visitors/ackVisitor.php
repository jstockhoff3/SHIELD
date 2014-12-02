<?php

include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");


$time=$_GET['time'];
$known=$_GET['known'];
$name=$_GET['name'];
$permit=$_GET['permit'];

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error')); 
$query = "SELECT * FROM AllVisitors WHERE TimeVisited='$time';";
$sth = mysqli_query($con,$query);
$r = mysqli_fetch_assoc($sth);

if($r==null){
	shield_output('None',0,'Visitor Not Found');
}
else{
	$impath=$r["Image"];
	if($time==null or $known==null){
		shield_output('None',0,'Missing Inputs');
	}
	else if($known==0){
		$query="UPDATE AllVisitors SET ACK=1,Known=0 WHERE TimeVisited='$time';";
		mysqli_query($con,$query);
		shield_output('Visitor Unknown and Acknowledged',1,'None');
	}
	else if($known==1){
		$query="UPDATE AllVisitors SET ACK=1,Known=1 WHERE TimeVisited='$time';";
		mysqli_query($con,$query);
		if($name==null or $permit==null){
			shield_output('None',0,'Missing inputs for known person');
		}
		else if($permit==0){
			$query="INSERT INTO KnownPersons (Name,LastVisit,Image,Permissions) VALUES ('$name','$time','$impath',0);";
			mysqli_query($con,$query);
			shield_output('Vistor known and not permitted',1,'None');
		}
		else if($permit==1){
			$query="INSERT INTO KnownPersons (Name,LastVisit,Image,Permissions) VALUES ('$name','$time','$impath',1);";
			mysqli_query($con,$query);
			shield_output('Vistor known and permitted',1,'None');
		}
	}
}

mysqli_close($con);

?>