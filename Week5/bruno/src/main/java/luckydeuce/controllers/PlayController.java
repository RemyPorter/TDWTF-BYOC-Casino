package luckydeuce.controllers;

import java.util.Map;
import luckydeuce.AppSingleton;
import luckydeuce.games.LegsBrokenException;
import luckydeuce.games.NotEnoughMoneyException;
import luckydeuce.players.User;
import luckydeuce.servlet.BaseController;
import luckydeuce.servlet.CasinoException;

public class PlayController extends BaseController {

    public PlayController() {
        params.add("action");
        params.add("invalid");
        params.add("game");
        params.add("play");
        params.add("username");
        params.add("amount");
    }

    public String handle(Map<String, String> params) {
        if (params.get("invalid") != null) {
            throw new RuntimeException();
        }
        String action = params.get("action");
        if (action.equals("play")) {
            try {
                double win = user.bet(params.get("play"));
                response.put("message", "YOU WON " + win);
            } catch (NotEnoughMoneyException e) {
                response.put("message", "Not enough money!");
            } catch (LegsBrokenException e) {
                throw new CasinoException(e.getMessage(), user);
            }
        } else if (action.equals("change")) {
            user.changeGame(Integer.parseInt(params.get("game")));
        } else if (action.equals("send")) {
            String username = params.get("username");
            Double amount = (params.get("amount") == null) ? null : Double.parseDouble(params.get("amount"));
            User target = AppSingleton.instance().getUserRegistry().find(username);
            if (!user.equals(target)) {
                user.sendMoney(target, amount);
            }
        }
        return "main.jsp";
    }

    @Override
    public boolean loginRequired() {
        return true;
    }

}
