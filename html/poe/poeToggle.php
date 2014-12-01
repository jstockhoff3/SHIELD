<?php
include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");

$name=$_GET['entryName'];

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error'));
$query = "SELECT * FROM PointsOfEntry WHERE EntryName='$name';";
$sth = mysqli_query($con,$query);

if($name==null){
	shield_output('None',0,'Missing entryName');
}
else{
	if($sth){
		if($r = mysqli_fetch_assoc($sth)){
			$locked = $r['Locked'];
			if($locked==1){
				$query=
				"UPDATE PointsOfEntry
				SET Locked=0
				WHERE EntryName='$name'";
				mysqli_query($con,$query);
				shield_output("$name is now Unlocked",1,'None');
			}
			else if($locked==0){
				$query=
				"UPDATE PointsOfEntry
				SET Locked=1
				WHERE EntryName='$name'";
				mysqli_query($con,$query);
				shield_output("$name is now Locked",1,'None');
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