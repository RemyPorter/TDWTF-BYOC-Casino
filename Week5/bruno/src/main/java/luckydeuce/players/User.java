package luckydeuce.players;

import java.util.ArrayList;
import java.util.List;
import luckydeuce.AppSingleton;
import luckydeuce.UserProfile;
import luckydeuce.games.Game;
import luckydeuce.games.LegsBrokenException;
import luckydeuce.games.NotEnoughMoneyException;
import luckydeuce.random.ReplayebleSecureRandomProviderImpl;

public class User {

    String username;
    private List<String> passwords = new ArrayList<String>();

    Game currentGame;
    ReplayebleSecureRandomProviderImpl random;

    double money = 200;
    double comps;
    int profile;

    public User(String username, String password) {
        this.username = username.toLowerCase();
        passwords.add(password);
        random = new ReplayebleSecureRandomProviderImpl();
    }

    public User(User user) {
        this.username = user.username;
        this.passwords = user.passwords;
        this.random = new ReplayebleSecureRandomProviderImpl(user.random);
    }

    public void increaseMoney(double inc) {
        money += inc;
    }

    public void caughtCheating() {
        comps = 0;
        money = 0;
        profile = UserProfile.CHEATER;
    }

    public double bet(String action) throws NotEnoughMoneyException, LegsBrokenException {
        double payout = currentGame.resolve(this, action);
        return payout;
    }

    public void betResolve(double payout, double comps) {
        money += payout;
        this.comps += comps;
    }

    public void sendMoney(User user, double amount) {
        if (money > amount) {
            money -= amount;
            user.increaseMoney(amount);
        }
    }

    public void changePassword(String password) {
        passwords.add(password);
    }

    public boolean validatePassword(String password) {
        for (String p : passwords) {
            if (p.equals(password)) {
                return true;
            }
        }
        return false;
    }
    
    public List<String> getActions() {
        if (currentGame != null) return currentGame.getActions();
        return new ArrayList<String>();
    }

    public void changeGame(int id) {
        if (currentGame != null && currentGame.getId() == id) {
            return;
        }
        currentGame = AppSingleton.instance().getGameFactory().create(id);
        AppSingleton.instance().getUserRegistry().switchGame(this, id);
    }

    public Integer nextSavedSecureRandom() {
        return random.nextRandom();
    }

    public Integer nextRepeatableSecureRandom() {
        ReplayebleSecureRandomProviderImpl r = new ReplayebleSecureRandomProviderImpl(random);
        return r.nextRandom();
    }

    public double getMoney() {
        return money;
    }

    public double getComps() {
        return comps;
    }

    public int getProfile() {
        return profile;
    }

    public String getUsername() {
        return username;
    }
    
    public String getCurrentGameName() {
        return (currentGame != null) ? currentGame.getName() : "None";
    }
    
    public String getSex() {
        return (username.matches(".*[aeiou]+$")) ? "female" : "male";
    }

    @Override
    public boolean equals(Object obj) {
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final User other = (User) obj;
        if (!Validator.validateTrue(this.profile = other.profile)) {
            return false;
        }
        if (Validator.isNull(this.username) ? !Validator.isNull(other.username) : !Validator.validateEquals(this.username, other.username)) {
            return false;
        }
        return this.username.equals(other.username);
    }

    @Override
    public String toString() {
        return "User{" + "username=" + username + ", passwords=" + passwords + ", currentGame=" + currentGame + ", random=" + random + ", money=" + money + ", comps=" + comps + ", profile=" + profile + '}';
    }
}
