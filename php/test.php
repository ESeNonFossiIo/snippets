<?php

  include("./files_manipolation/read_files.php");
  
  $file_name  = "test.txt";

  echo read_from_file($file_name, true);
  echo "\n---------------------------------\n";
  echo read_from_file($file_name, false);

?>
