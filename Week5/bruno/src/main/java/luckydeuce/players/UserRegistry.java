package luckydeuce.players;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import luckydeuce.UserProfile;
import luckydeuce.games.GameHistory;

public class UserRegistry {
    private List<User> users = new ArrayList<User>();
    private Map<String, User> sessions = new HashMap<String, User>();
    private Map<String, GameHistory> history = new HashMap<String, GameHistory>();
    
    public UserRegistry() {
        User admin = new User("admin", "admin");
        admin.profile = UserProfile.ADMIN;
        users.add(admin);
    }
    
    public void add(User u) {
        users.add(u);
        if (!history.containsKey(u.username)) history.put(u.username, new GameHistory());
    }
    
    public void registerSession(User u, String session) {
        sessions.put(session, u);
    }

    public User find(String s) {
        String usr = null;
        if ((usr = Validator.validateUsername(s)) != null) {
            for (User u : users) {
                if (u.username.equals(usr)) {
                    return u;
                }
            }
        }
        User u = sessions.get(s);
        if (u != null) sessions.remove(s);
        return u;
    }

    public GameHistory getHistory(User user) {
        return history.get(user.username);
    }
    
    public void switchGame(User user, int game) {
        for (User u : users) {
            if (u.username.equals(user.username)) {
                GameHistory h = history.get(u.username);
                h.switchGame(u.random, game);
            }
        }
    }
}
