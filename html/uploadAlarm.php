<?php

include(dirname(__FILE__)."/libs/shield_DB_info.php");
include(dirname(__FILE__)."/libs/shield_PHP_lib.php");


$target_dir = "images/newcaptures/";
$target_file = $target_dir . basename($_FILES["fileToUpload"]["name"]);
$uploadOk = 1;
$imageFileType = pathinfo($target_file,PATHINFO_EXTENSION);
// Check if image file is a actual image or fake image
if(isset($_POST["submit"])) {
    $check = getimagesize($_FILES["fileToUpload"]["tmp_name"]);
    if($check !== false) {
        //echo "File is an image - " . $check["mime"] . ".";
        $uploadOk = 1;
    } else {
        echo "File is not an image.\n";
        $uploadOk = 0;
    }
}
// Check if file already exists
if (file_exists($target_file)) {
    //echo "File exists, overwriting it.";
    $uploadOk = 1;
}
// Check file size
if ($_FILES["fileToUpload"]["size"] > 1000000) {
    echo "Sorry, your file is too large.\n";
    $uploadOk = 0;
}
// Allow certain file formats
if($imageFileType != "jpg" && $imageFileType != "png" && $imageFileType != "jpeg"
&& $imageFileType != "gif" ) {
    echo "Sorry, only JPG, JPEG, PNG & GIF files are allowed.\n";
    $uploadOk = 0;
}
// Check if $uploadOk is set to 0 by an error
if ($uploadOk == 0) {
    echo "Sorry, your file was not uploaded.";
// if everything is ok, try to upload file
} else {
    if (move_uploaded_file($_FILES["fileToUpload"]["tmp_name"], $target_file)) {
        echo "The file ". basename( $_FILES["fileToUpload"]["name"]). " has been uploaded.\n";
    } else {
        echo "Sorry, there was an error uploading your file.\n";
    }
}

$new = "alarm.png";
$new = $target_dir . basename($new);
rename($target_file,$new);

// $image=$new;

// $con = mysqli_connect($mysql_host,$mysql_user,$mysql_password,$mysql_database) or die(shield_output('None',0,'Database Connection Error'));
// $res = mysqli_query($con,"SELECT * FROM AllVisitors");

// if($image==null){
//     shield_output('None',0,'No image path entered');
// }
// else{   
//     if($res){
//         $query=
//         "INSERT INTO AllVisitors (TimeVisited,Image) VALUES (NOW(),'$image');";
//         mysqli_query($con,$query);
//         shield_output("Visitor created",1,'None');
//     }
//     else{
//         shield_output('None',0,'Query Error');
//     }
// }


// mysqli_close($con);

?>