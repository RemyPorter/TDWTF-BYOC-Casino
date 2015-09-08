<%@page import="luckydeuce.players.User"%>
<%@page import="luckydeuce.games.GameFactory"%>
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
            User: ${user.username}<br>
            Selected game: ${user.currentGameName}<br>
            Current money: ${user.money}<br>
            Comps: ${user.comps}<br>
            <happy>${message}</happy><br>
            Play game: <br>
            <form method="GET" action="play.casino">
                <input type="hidden" name="i" value="${i}"/>
                <select name="play">
                    <% List<String> actions = ((User)request.getAttribute("user")).getActions();
                    for (String s : actions) { %>
                    <option value="<%=s%>"><%=s%></option>
                    <% } %>
                </select>
                <input type="submit" name="action" value="play"/>
            </form>
                Change game: <br>
            <form method="GET" action="play.casino">
                <input type="hidden" name="i" value="${i}"/>
                <select name="game">
                    <% List<String> games = ((GameFactory) request.getAttribute("games")).getGameNames();
                    for (int i = 0; i < games.size(); i++) { %>
                    <option value="<%=i%>"><%=games.get(i)%></option>
                    <% } %>
                </select>
                <input type="submit" name="action" value="change"/>
            </form>
                Send money: <br>
                <form method="GET" action="play.casino">
                    <input type="hidden" name="i" value="${i}"/>
                    User: <input type="text" name="username"/>
                    Amount: <input type="text" name="amount" />
                    <input type="submit" name="action" value="send" />
                </form>
        </main>
    </body>
</html>