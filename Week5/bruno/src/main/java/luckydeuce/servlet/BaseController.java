package luckydeuce.servlet;

import java.math.BigInteger;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import javax.servlet.http.HttpServletRequest;
import luckydeuce.AppSingleton;
import luckydeuce.players.User;

public abstract class BaseController implements Controller {
    protected List<String> params = new ArrayList<String>();
    protected Map<String, Object> response = new HashMap<String, Object>();
    protected User user;
    
    String handle(HttpServletRequest req, Map<String, String> values) throws NoSuchAlgorithmException {
        if (req.getParameter("i") != null) {
            String session = req.getParameter("i");
            if (session.length() != 32) throw new RuntimeException("Tampering detected!");
            user = AppSingleton.instance().getUserRegistry().find(session);
            req.setAttribute("user", user);
            req.setAttribute("games", AppSingleton.instance().getGameFactory());
        } else if (loginRequired()) throw new RuntimeException("Verbotten!");
        
        String resp = handle(values);
        if (!loginRequired()) {
            // new user
            AppSingleton.instance().getUserRegistry().add(user);
        }
        
        String newSessionId = user.nextRepeatableSecureRandom().toString();
        
        MessageDigest md = MessageDigest.getInstance("MD5");
        md.update(newSessionId.getBytes(),0,newSessionId.length());
        newSessionId = new BigInteger(1,md.digest()).toString(16);
        if (newSessionId.length() < 32) {
            while(newSessionId.length() != 32) newSessionId = "0" + newSessionId;
        }
        
        req.setAttribute("i", newSessionId);
        AppSingleton.instance().getUserRegistry().registerSession(user, newSessionId);
        return resp;
    }

    public Map<String, Object> getResponse() {
        return response;
    }

    public List<String> getParameters () {
        return params;
    }
    
    public boolean loginRequired() {
        return false;
    }
}
