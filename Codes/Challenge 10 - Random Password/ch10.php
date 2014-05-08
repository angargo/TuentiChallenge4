<?php
  //Reads input from file and prints the web's answer
  $input = file_get_contents(STDIN);
  date_default_timezone_set("GMT");
  srand(mktime(date("H"), date("i"), 0) * 1336);
  $url = "http://random.contest.tuenti.net/?input=" . $input . "&password=" . rand();
  $result = file_get_contents($url);
  echo $result . "\n";
?>
