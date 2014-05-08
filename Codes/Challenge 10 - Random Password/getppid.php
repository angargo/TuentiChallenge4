<?php
//Bruteforces until finding ppid
for ($i = 0; $i < 50000; $i++) {
  date_default_timezone_set("GMT");
  srand(mktime(date("H"), date("i"), 0) * $i);
  $url = "http://random.contest.tuenti.net/?input=bfd84a3611&password=" . rand();
  $result = file_get_contents($url);
  if ($result != "wrong!") {
    file_put_contents("ppid.txt", $i);
    break;
  }
}
?>
