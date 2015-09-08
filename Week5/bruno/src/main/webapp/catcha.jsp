<%@page import="java.util.List"%>
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
            <form method="GET" action="play.casino">
                <% for (String c : (List<String>)request.getAttribute("catcha")) { %>
                <img src="<%=c%>" height="100" width="100"/>
                <% } %><br>
                <input type="hidden" name="i" value="${i}"/>
                The first cat is black?<br>
                <input type="submit" name="${one}" value="TRUE"/>
                <input type="submit" name="${two}" value="FALSE"/>
            </form>
        </main>
    </body>
</html>