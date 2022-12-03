
<!DOCTYPE html>
<html lang="en">
<head>
	<meta charset="UTF-8">
	<meta http-equiv="X-UA-Compatible" content="IE=edge">
	<meta name="viewport" content="width=device-width, initial-scale=1.0">
	<title>Login System</title>
</head>
<body>
	<?php 
	if (isset($_POST['username']) || isset($_POST['pass']))
		include_once 'login.php' 
	?>
	<form action="login.php" method="post">
		<label for="name">Username:</label>
		<input type="text" id="name" name="username">
		<label for="pass">Password:</label>
		<input type="password" id="password" name="pass">
		<input type="submit" value="submit">
	</form>
	<input type="submit" value="SHOW DB">
</body>
</html>
