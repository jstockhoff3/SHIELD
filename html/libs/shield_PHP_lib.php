<?php

include(dirname(__FILE__)."shield_DB_info.php");

function shield_output($data,$success,$err){
	$output = array();
	$output[0] = $data;
	$output[1] = array(
		'Successful' => "$success",
		'Error' => $err
		);
	print(json_encode($output));
}

function motionAlarmTrigger($link,$sensor){
	$query = "SELECT MotionFound,TriggerAlarm,SystemName FROM Motion WHERE Name='$sensor';";
	$res = mysqli_query($link,$query);
	$res = mysqli_fetch_assoc($res);
	$triggerAlarm = $res['TriggerAlarm'];
	$systemName = $res['SystemName'];
	$motionFound = $res['MotionFound'];
	$query = "SELECT Armed,AlarmActive FROM System WHERE Name='$systemName';";
	$res = mysqli_query($link,$query);
	$res = mysqli_fetch_assoc($res);
	$alarmactive = $res['AlarmActive'];
	$systemArmed = $res['Armed'];
	if($motionFound){
		if($triggerAlarm){
			if($systemArmed){
				if($alarmactive==0){
					$query = "UPDATE System SET AlarmActive=1 WHERE Name='$systemName';";
					mysqli_query($link,$query);
					$query = "INSERT INTO AlarmHistory (AlarmTime,Image,SystemName,ACK,TriggeredBy) VALUES (NOW(),'images/motion.jpg','$systemName',0,'$sensor');";
					mysqli_query($link,$query);
					return 1;
				}
			}
			else{
				return 0;
			}
		}
		else{
			return 0;
		}
	}
	else{
		return 0;
	}
}

function poeAlarmTrigger($con){

	$rows = array();

	//$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error'));
	$query = "SELECT * FROM PointsOfEntry;";
	$sth = mysqli_query($con,$query);

	if($sth){ 
		while($r = mysqli_fetch_assoc($sth)) 
		{
		    $sysname = $r["SystemName"];
		    $entname = $r["EntryName"];
		    $open = $r["Open"];
		    $query = "SELECT * FROM System WHERE Name='$sysname';";
		    $sth = mysqli_query($con,$query);
		    $r = mysqli_fetch_assoc($sth);
		    $armed = $r["Armed"];
		    if($armed==1){
		    	if($open==1){
		    		$query = "UPDATE System SET AlarmActive=1 WHERE Name='$sysname';";
					mysqli_query($con,$query);
					$query = "INSERT INTO AlarmHistory (AlarmTime,Image,SystemName,ACK,TriggeredBy) VALUES (NOW(),'images/doorOpen.jpg','$sysname',0,'$entname');";
					mysqli_query($con,$query);
		    	}
		    }
		}
	}
	else{
		shield_output('None',0,'Query Error');
	}

}

function generateRandomString($length = 10) {
    $characters = '0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ';
    $charactersLength = strlen($characters);
    $randomString = '';
    for ($i = 0; $i < $length; $i++) {
        $randomString .= $characters[rand(0, $charactersLength - 1)];
    }
    return $randomString;
}

?>