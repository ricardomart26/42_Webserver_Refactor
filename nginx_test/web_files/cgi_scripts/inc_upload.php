
<?php

function my_error_handling($location, $errorMsg)
{
	header("Location: ".$location.'?'.$errorMsg);
	exit();
}

$dir = "uploads/";

if (!isset($_POST["submit"]))
	my_error_handling("../upload.php", "SubmitNotClicked");

if ($_FILES["filename"]["error"] !== 0)
	my_error_handling("../upload.php", "ErrorUploadingFile");


$fileArray = explode('.', $_FILES["filename"]["name"]);
$fileExt = strtolower(end($fileArray));

$allowedExt = array('jpg', 'jpeg', 'txt', 'png', 'pdf');

if (!in_array($fileExt, $allowedExt))
	my_error_handling("../upload.php", "WrongExtension");

if ($_FILES["filename"]["size"] >= 50000)
	my_error_handling("../upload.php", "FileToBig");

if ($_FILES["filename"]["size"] === 0)
	my_error_handling("../upload.php", "EmptyFile");

move_uploaded_file($_FILES["filename"]["tmp_name"], $dir.uniqid('', true).".".$fileExt);
header('Location: ../upload.php?uploadSuccess');

?>
