package luckydeuce.controllers;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import luckydeuce.AppSingleton;
import luckydeuce.UserProfile;
import luckydeuce.players.User;
import luckydeuce.players.Validator;
import luckydeuce.servlet.BaseController;
import luckydeuce.servlet.CasinoException;

public class LoginController extends BaseController {

    public LoginController() {
        params.add("username");
        params.add("password");
        params.add("login");
    }

    public String handle(Map<String, String> params) {
        User u = AppSingleton.instance().getUserRegistry().find(params.get("username"));
        if (u == null) {
            u = new User(Validator.validateUsername(params.get("username")), params.get("password"));
            AppSingleton.instance().getUserRegistry().add(u);
        }
        if (u.getProfile() == UserProfile.CHEATER) {
            throw new CasinoException("", u);
        }
        
        List<String> source = new ArrayList<String>();
        List<String> catcha = new ArrayList<String>();
        Integer random = u.nextSavedSecureRandom();
        System.out.println("catcha:" + random);
        for (int i = 1; i < 14; i++) {
            source.add("catcha/" + i + ".jpg");
        }
        for (int j = 13; j > 1; j--) {
            catcha.add(source.get(random % j));
            source.remove(random % j);
        }
        response.put("catcha", catcha);
        if (catcha.get(0).equals("1.jpg")) {
            response.put("one", "action");
            response.put("two", "invalid");
        } else {
            response.put("one", "invalid");
            response.put("two", "action");
        }

        if (user == null) {
            user = new User(u);
        }
        String x = user.getUsername();
        while (x.length() != 32) {
            x = x + " ";
        }
        return "catcha.jsp";
    }
}
