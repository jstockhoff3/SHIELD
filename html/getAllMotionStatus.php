<?php
include 'shield_DB_info.php';

$rows = array();

$con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die("Error " . mysqli_error($con)); 
$query = "SELECT * FROM Motion ORDER BY Name ASC;";
$sth = mysqli_query($con,$query) OR die("Invalid query: $query". mysql_error());


if($r = mysqli_fetch_assoc($sth)){
	$rows[] = $r;
	while($r = mysqli_fetch_assoc($sth)) 
	{
	    $rows[] = $r;
	}
	print json_encode($rows);
}
else{
	print json_encode("Table Empty");
}

mysqli_close($con)

?>