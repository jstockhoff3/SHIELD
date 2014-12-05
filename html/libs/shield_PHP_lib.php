<?php

include("shield_DB_info.php");

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

function openDoorAlarmTrigger($con){

	$query = "SELECT * FROM System where Name='Main';";
	$res = mysqli_query($con,$query);
	$res = mysqli_fetch_assoc($res);
	$armed = $res["Armed"];
	//$activealarm = $res["AlarmActive"];
	if($armed==1){
		$query = "UPDATE System SET AlarmActive=1 WHERE Name='Main';";
		mysqli_query($con,$query);
		//if($activealarm==0){
			$query = "INSERT INTO AlarmHistory (AlarmTime,Image,SystemName,ACK,TriggeredBy) VALUES (NOW(),'images/doorOpen.jpg','Main',0,'Front_Door');";
			mysqli_query($con,$query);
		//}
	}


	$query = "SELECT * FROM PointsOfEntry where EntryName='Front_Door';";
	$res = mysqli_query($con,$query);
	$res = mysqli_fetch_assoc($res);
	$locked = $res["Locked"];
	if($armed==1){
		$query = "UPDATE System SET AlarmActive=1 WHERE Name='Main';";
		mysqli_query($con,$query);
		//if($activealarm==0){
			$query = "INSERT INTO AlarmHistory (AlarmTime,Image,SystemName,ACK,TriggeredBy) VALUES (NOW(),'images/doorOpen.jpg','Main',0,'Front_Door');";
			mysqli_query($con,$query);
		//}
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
		    $query = "SELECT * FROM System WHERE Name='Main';";
		    $sth = mysqli_query($con,$query);
		    $r = mysqli_fetch_assoc($sth);
		    $activealarm = $r["AlarmActive"];
		    $armed = $r["Armed"];
		    if($armed==1){
		    	if($open==1){
		    		$query = "UPDATE System SET AlarmActive=1 WHERE Name='Main';";
					mysqli_query($con,$query);
					if($activealarm==0){
						$query = "INSERT INTO AlarmHistory (AlarmTime,Image,SystemName,ACK,TriggeredBy) VALUES (NOW(),'images/doorOpen.jpg','$sysname',0,'$entname');";
						mysqli_query($con,$query);
					}
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

function getAlarmDate(){

	global $mysql_host;
	global $mysql_password;
	global $mysql_user;
	global $mysql_database;

	$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error')); 
	$query = "SELECT * FROM AlarmHistory WHERE ACK=0;";
	$sth = mysqli_query($con,$query);
	$r = mysqli_fetch_assoc($sth);

	if($r==null){
		mysqli_close($con);
		return 0;
	}
	else{
		mysqli_close($con);
		return $r["AlarmTime"];
	}

}

?>