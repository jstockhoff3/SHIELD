<?php

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
	$query = "SELECT Armed FROM System WHERE Name='$systemName';";
	$res = mysqli_query($link,$query);
	$res = mysqli_fetch_assoc($res);
	$systemArmed = $res['Armed'];
	if($motionFound){
		if($triggerAlarm){
			if($systemArmed){
				$query = "UPDATE System SET AlarmActive=1 WHERE Name='$systemName';";
				mysqli_query($link,$query);
				$query = "INSERT INTO AlarmHistory (AlarmTime,Image,SystemName,ACK,TriggeredBy) VALUES (NOW(),'/test/path/img.jpg','$systemName',0,'$sensor');";
				mysqli_query($link,$query);
				return 1;
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

?>