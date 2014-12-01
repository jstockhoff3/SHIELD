<?php
include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");

$rows = array();

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error')); 
$query = "SELECT * FROM PointsOfEntry ORDER BY EntryName ASC;";
$sth = mysqli_query($con,$query);

if($sth){ 
	if($r = mysqli_fetch_assoc($sth)){
		$rows[] = $r;
		while($r = mysqli_fetch_assoc($sth)) 
		{
		    $rows[] = $r;
		}
		shield_output($rows,1,'None');
	}
	else{
		shield_output('None',0,'Table Empty');
	}
}
else{
	shield_output('None',0,'Query Error');
}

mysqli_close($con);

?>