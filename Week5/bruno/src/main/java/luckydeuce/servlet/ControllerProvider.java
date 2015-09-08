package luckydeuce.servlet;

import luckydeuce.controllers.LoginController;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import luckydeuce.controllers.PlayController;

class ControllerProvider {

    // thread safety!
    private final Map<String, BaseController> controllers = new ConcurrentHashMap<String, BaseController>();

    ControllerProvider() {
        controllers.put("/login.casino", new LoginController());
        controllers.put("/play.casino", new PlayController());
    }

    BaseController find(String url) {
        BaseController c = controllers.get(url);
        if (c != null) return c;
        return controllers.get("login");
    }
}
