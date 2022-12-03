<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Document</title>
</head>
<body>
	<h1>Hello</h1>
	<form action="inc_upload.php" method="POST" enctype="multipart/form-data">
		<input type="file" name="filename">
		<button type="submit" name="submit">UPLOAD FILE</button>
	</form>
	<?php
		if (isset($_GET["uploadSuccess"]))
			echo "\nUpload successfull";
		if (isset($_GET["EmptyFile"]))
			echo "\nFile not uploaded: File is empty";
		if (isset($_GET["WrongExtension"]))
			echo "\nFile not uploaded: Wrong extension";
	?>
</body>
</html>

