<?php
$myfile = fopen("newfile.html", "w") or die("Unable to open file!");



$txt = "<html>
<head>
<style>
div.img {
    margin: 5px;
    padding: 5px;
    border: 1px solid #0000ff;
    height: auto;
    width: auto;
    float: left;
    text-align: center;
}

div.img img {
    display: inline;
    margin: 5px;
    border: 1px solid #ffffff;
}

div.img a:hover img {
    border:1px solid #0000ff;
}

div.desc {
    text-align: center;
    font-weight: normal;
    width: 120px;
    margin: 5px;
}
</style>
</head>
<body>

<div class="img">
  <a target="_blank" href="klematis_big.htm">
    <img src="klematis_small.jpg" alt="Klematis" width="110" height="90">
  </a>
  <div class="desc">Add a description of the image here</div>
</div>
<div class="img">
  <a target="_blank" href="klematis2_big.htm">
    <img src="klematis2_small.jpg" alt="Klematis" width="110" height="90">
  </a>
  <div class="desc">Add a description of the image here</div>
</div>
<div class="img">
  <a target="_blank" href="klematis3_big.htm">
    <img src="klematis3_small.jpg" alt="Klematis" width="110" height="90">
  </a>
  <div class="desc">Add a description of the image here</div>
</div>
<div class="img">
  <a target="_blank" href="klematis4_big.htm">
    <img src="klematis4_small.jpg" alt="Klematis" width="110" height="90">
  </a>
  <div class="desc">Add a description of the image here</div>
</div>

</body>
</html>";

print 'made it';


fwrite($myfile, $txt);
fclose($myfile);
?>