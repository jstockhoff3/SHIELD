<?php
include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");

$name=$_GET['systemName'];

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error'));
$query = "SELECT * FROM System WHERE Name='$name';";
$sth = mysqli_query($con,$query);

if($name==null){
	shield_output('None',0,'Missing systemName');
}
else{
	if($sth){
		if($r = mysqli_fetch_assoc($sth)){
			$armed = $r['Armed'];
			if($armed==1){
				$query=
				"UPDATE System
				SET Armed=0,AlarmActive=0
				WHERE Name='$name'";
				mysqli_query($con,$query);
				$query=
				"UPDATE AlarmHistory
				SET ACK=1
				WHERE SystemName='$name'";
				mysqli_query($con,$query);
				shield_output("$name is now Disarmed",1,'None');
			}
			else if($armed==0){
				$query=
				"UPDATE System
				SET Armed=1
				WHERE Name='$name'";
				mysqli_query($con,$query);
				shield_output("$name is now Armed",1,'None');
			}
			else{
				shield_output('None',0,'SQL Update Query Error');
			}
		}
		else{
			shield_output('None',0,'System Not Found');
		}
	}
	else{
		shield_output('None',0,'Initial Query Error');
	}
}

mysqli_close($con)

?>