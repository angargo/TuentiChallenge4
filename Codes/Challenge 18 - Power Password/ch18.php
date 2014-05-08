<?
$input = fgets(STDIN);
$url = 'http://54.83.207.90:9083/index.py?' . $input . ':0';
$result = file_get_contents($url);
echo $result;
?>