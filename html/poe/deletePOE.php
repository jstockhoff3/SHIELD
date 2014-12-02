<?php
include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");

$name=$_GET['poeName'];

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error'));
$res = mysqli_query($con,"SELECT * FROM PointsOfEntry WHERE EntryName='$name'");
	
if($name==null){
	shield_output('None',0,'Missing poeName');
}
else{
	if($res){
		if(mysqli_num_rows($res) == 0){
			shield_output('None',0,'POE Not Found');
		}
		else{
			$query=
			"DELETE FROM PointsOfEntry WHERE EntryName='$name';";
			mysqli_query($con,$query);
			shield_output("Entry $name deleted",1,'None');
		}
	}
	else{
		shield_output('None',0,'Initial Query Error');
	}
}

mysqli_close($con);

?>