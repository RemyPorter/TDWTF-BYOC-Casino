<%@page contentType="text/html" pageEncoding="UTF-8"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>JSP Page</title>
        <link rel="stylesheet" type="text/css" media="screen" href="css.css" />
    </head>
    <body>
        <main class="${user.sex}">
            <form method="GET" action="login.casino">
                Username (4-8 chars): <input type="text" name="username" value=""/>
                Password: <input type="password" name="password" value=""/>
                <input type="submit" name="login" value="login"/>
            </form>
        </main>
    </body>
</html>
