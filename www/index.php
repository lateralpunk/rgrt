<?php

  //ensure that the script doesn't time out
set_time_limit(0);

if (isset($_POST['doit'])) {
  //defaults
  $build = "build02";
  $width = $_POST['width'];
  $height = $_POST['height'];

  $n = 1;
  $z = 1;
  $p = 1;
  $o = "/rgrt/www/images/" . uniqid('rgrt-', true) . "-rgrt.bmp";

  //ok execute the rgrt-cuda program.  
  echo shell_exec("export LD_LIBRARY_PATH=/usr/local/cuda-5.0/lib:/usr/local/cuda-5.0/lib64:\$LD_LIBRARY_PATH;cd /rgrt;bin/linux/release/rgrt-cuda m=1 -g=1 -b=" . $build . " -w=" . $width . " -h=" . $height . " -n=" . $n . " -z=" . $z . " -p=" . $p . " -o=" . $o);

  //now use the Linux at command to delete the newly created file after 15 minutes
  shell_exec("export SHELL=/bin/bash && echo rm -f " . $o . " | at now + 15 minutes");



  exit;

}


?>

<!doctype html>
 
<html lang="en">
<head>
  <meta charset="utf-8" />
  <title>Regular-Grid Accelerated Ray Tracing</title>
  <link rel="stylesheet" href="http://code.jquery.com/ui/1.9.2/themes/base/jquery-ui.css" />
  <script src="http://code.jquery.com/jquery-1.8.3.js"></script>
  <script src="http://code.jquery.com/ui/1.9.2/jquery-ui.js"></script>
  <link rel="stylesheet" href="style.css" />
  <script>
  $(function() {
      $( "#slider-width" ).slider({
	range: "max",
	    min: 16,
	    max: 8192,
	    value: 512,
	    step: 4,
	    slide: function( event, ui ) {
	    $( "#width" ).val( ui.value );
	  }
	});
      $( "#width" ).val( $( "#slider-width" ).slider( "value" ) );
    });
  $(function() {
      $( "#slider-height" ).slider({
	range: "max",
	    min: 16,
	    max: 8192,
	    value: 512,
	    step: 4,
	    slide: function( event, ui ) {
	    $( "#height" ).val( ui.value );
	  }
	});
      $( "#height" ).val( $( "#slider-height" ).slider( "value" ) );
    });
  $(function() {
      $( "#slider-samples" ).slider({
	range: "max",
	    min: 1,
	    max: 256,
	    value: 1,
	    step: 1,
	    slide: function( event, ui ) {
	    $( "#samples" ).val( ui.value );
	  }
	});
      $( "#samples" ).val( $( "#slider-samples" ).slider( "value" ) );
    });


  </script>
</head>
<body>
 
<form action="index.php" method="post">
<input type="hidden" name="doit" value="doit">

<p>
  <label>Width:</label>
  <input type="text" id="width" name="width" style="border: 0; color: #f6931f; font-weight: bold;" />
</p>
<div id="slider-width"></div>

<p>
  <label>Height:</label>
  <input type="text" id="height" name="height" style="border: 0; color: #f6931f; font-weight: bold;" />
</p>
<div id="slider-height"></div>

<p>
  <label>Samples:</label>
  <input type="text" id="samples" name="samples" style="border: 0; color: #f6931f; font-weight: bold;" />
</p>
<div id="slider-samples"></div>





<br/>

<input type="submit" value="Submit">

</form>
 
 
</body>
</html>
