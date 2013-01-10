<?php

  //ensure that the script doesn't time out
set_time_limit(0);

if (isset($_POST['doit'])) {
  //defaults
  $build = $_POST['build'];
  $g = $_POST['grid'];
  $width = $_POST['width'];
  $height = $_POST['height'];
  $n = $_POST['samples'];
  $z = $_POST['zoom'];
  $o = "/rgrt/www/images/" . uniqid('rgrt-', true) . "-rgrt.bmp";

  //ok execute the rgrt-cuda program.  
  shell_exec("export LD_LIBRARY_PATH=/usr/local/cuda-5.0/lib:/usr/local/cuda-5.0/lib64:\$LD_LIBRARY_PATH;cd /rgrt;bin/linux/release/rgrt-cuda m=1 -g=". $g ." -b=" . $build . " -w=" . $width . " -h=" . $height . " -n=" . $n . " -z=" . $z . " -p=0 -o=" . $o);

  //now use the Linux at command to delete the newly created file after 15 minutes
  shell_exec("export SHELL=/bin/bash && echo rm -f " . $o . " | at now + 15 minutes");

  //sleep for a sec
  sleep(2);

  //now show the user the image:
  header('Location: images' . strrchr($o,'/'));

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
	    max: 4096,
	    value: 512,
	    step: 2,
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
	    max: 4096,
	    value: 512,
	    step: 2,
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

  $(function() {
      $( "#slider-zoom" ).slider({
	range: "max",
	    min: -1024,
	    max: 1024,
	    value: 1,
	    step: 1,
	    slide: function( event, ui ) {
	    $( "#zoom" ).val( ui.value );
	  }
	});
      $( "#zoom" ).val( $( "#slider-zoom" ).slider( "value" ) );
    });


  </script>
</head>
<body>
 
<h1>More information about the implementation of this application can be found <a href='http://www.cs264.org/2009/projects/web/Patel_Krunal/' target="_blank">here</a>.</h1>

<form action="index.php" method="post">
<input type="hidden" name="doit" value="doit">

<p>
<label>Build File:</label>
<select name="build">
<option value="build01">1</option>
<option value="build02">2</option>
<option value="build03">3</option>
<option value="build04">4</option>
<option value="build05">5</option>
<option value="build06">6</option>
<option value="build07">7</option>
<option value="build08">8</option>
<option value="build09">9</option>
</select>
</p>

<p>
<label>Grid Acceleration:</label>
<select name="grid">
<option value="1">Yes</option>
<option value="0">No</option>
</select>
</p>

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

<p>
  <label>Zoom:</label>
  <input type="text" id="zoom" name="zoom" style="border: 0; color: #f6931f; font-weight: bold;" />
</p>
<div id="slider-zoom"></div>

<br/>
<input type="submit" value="Submit">

</form>
 
 
</body>
</html>
