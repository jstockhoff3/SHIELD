<?php
include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");

$name=$_GET['entryName'];
$status=$_GET['action'];

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error'));
$query = "SELECT * FROM PointsOfEntry WHERE EntryName='$name';";
$sth = mysqli_query($con,$query);

if($name==null or action==null ){
	shield_output('None',0,'Missing inputs');
}
else{
	if($sth){
		if($r = mysqli_fetch_assoc($sth)){
			if($status=="lock"){
				$query=
				"UPDATE PointsOfEntry
				SET Locked=1
				WHERE EntryName='$name'";
				mysqli_query($con,$query);
				shield_output("$name is now locked",1,'None');
			}
			else if($status=="unlock"){
				$query=
				"UPDATE PointsOfEntry
				SET Locked=0
				WHERE EntryName='$name'";
				mysqli_query($con,$query);
				shield_output("$name is now unlocked",1,'None');
			}
			else if($status=="open")
			{
				$query=
				"UPDATE PointsOfEntry
				SET Open=1
				WHERE EntryName='$name'";
				mysqli_query($con,$query);
				shield_output("$name is now open",1,'None');
			}
			else if($status=="close"){
				$query=
				"UPDATE PointsOfEntry
				SET Open=0
				WHERE EntryName='$name'";
				mysqli_query($con,$query);
				shield_output("$name is now closed",1,'None');
			}
			else{
				shield_output('None',0,'SQL Update Query Error');
			}
		}
		else{
			shield_output('None',0,'Entry Not Found');
		}
	}
	else{
		shield_output('None',0,'Initial Query Error');
	}
}

mysqli_close($con)

?>