<?php

//sorts the AllVisitors table and gets most recently added visitor
//name & known inputs are optional, if known=1 then a name should be entered
// 	if known=1
//		visitor is added to KnownPersons with input name, lastvisit=time added to AllVisitors, image path is maintained, permissions=1 since known
//	if known=0
//		visitor is simply set to ACK=1 in AllVisitors table indicating visitor has been viewed

include(dirname(__FILE__)."/../libs/shield_DB_info.php");
include(dirname(__FILE__)."/../libs/shield_PHP_lib.php");

$name=$_GET['name'];
$known=$_GET['known'];

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error')); 
$query = "SELECT * FROM AllVisitors WHERE ACK=0 ORDER BY TimeVisited DESC LIMIT 1;";
$sth = mysqli_query($con,$query);

if($sth){ 
	if($r = mysqli_fetch_assoc($sth)){
		$image = $r['Image'];
		$lastvisit = $r['TimeVisited'];
		if($known==1){
			if($name==null){
				shield_output('None',0,'No name entered');
			}
			else{
				$query="UPDATE allvisitors SET ack=1,known='$known' WHERE ack=0 ORDER BY timevisited DESC LIMIT 1";
				mysqli_query($con,$query);
				$query= "INSERT INTO KnownPersons (Name,LastVisit,Image,Permissions) VALUES ('$name','$lastvisit','$image','$known');";
				mysqli_query($con,$query);
				shield_output('Visitor acknowledged and added to known list',1,'None');
			}
		}
		if($known==0 or $known==null){
			$query="UPDATE allvisitors SET ack=1 WHERE ack=0 ORDER BY timevisited DESC LIMIT 1";
			mysqli_query($con,$query);
			shield_output('Visitor acknowledged',1,'None');
		}
	}
	else{
		shield_output('None',0,'No New Visitors');
	}
}
else{
	shield_output('None',0,'Query Error');
}

mysqli_close($con);

?>