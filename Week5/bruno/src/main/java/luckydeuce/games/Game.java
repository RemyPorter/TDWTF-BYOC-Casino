package luckydeuce.games;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import luckydeuce.AppSingleton;
import luckydeuce.UserProfile;
import luckydeuce.players.User;

public class Game {
    String name;
    int id;
    String currentState = "";
    private Map<String, List<Action>> actions = new HashMap<String, List<Action>>();

    Game(List<GameFileData> data, int id, String name) {
        this.id = id;
        this.name = name;
        for (GameFileData d : data) {
            List<Action> aList = actions.get(d.state);
            if (aList == null) {
                aList = new ArrayList<Action>();
                actions.put(d.state, aList);
            }

            Action found = null;
            for (Action a : aList) {
                if (a.name.equals(d.action)) {
                    found = a;
                }
            }
            if (found == null) {
                found = new Action(d.action);
                aList.add(found);
            }
            found.addOutcome(new Outcome(d.newState, d.amountChange, d.start, d.width));
        }
    }

    public List<String> getActions() {
        List<String> l = new ArrayList<String>();
        for (Action a : actions.get(currentState)) {
            l.add(a.name);
        }
        return l;
    }

    public double resolve(User user, String action) throws NotEnoughMoneyException, LegsBrokenException {
        if (user.getProfile() == UserProfile.PLAYER) {
            boolean isCheating = AppSingleton.instance().getUserRegistry().getHistory(user).replay(id);
            if (isCheating) {
                user.caughtCheating();
                throw new LegsBrokenException();
            }
            Action a = find(action);
            double comps = calculateComps(a, actions.get(currentState));
            if (!a.isBetValid(user.getMoney())) {
                throw new NotEnoughMoneyException();
            }
            Integer random = user.nextSavedSecureRandom();
            System.out.println("game:" + random);
            double payout = resolve(a, random);
            user.betResolve(payout, comps);
            return payout;
        } else {
            return 0.;
        }
    }

    double resolve(Action action, Integer random) {
        Outcome o = action.resolve(random);
        currentState = o.newState;
        return o.amountChange;
    }

    Action find(String action) {
        for (Action a : actions.get(currentState)) {
            if (a.name.equals(action)) {
                return a;
            }
        }
        return null;
    }

    private double calculateComps(Action action, List<Action> actions) {
        // not awarding for non risk actions
        if (action.expectedPayout == 0.) {
            return 0.;
        }
        // 1 comp each round, award most to acion with biggest expected loss
        double expectedPayoutSum = 0.;
        for (Action a : actions) {
            if (a.expectedPayout != 0.) {
                expectedPayoutSum += a.expectedPayout;
            }
        }
        return action.expectedPayout / expectedPayoutSum;
    }

    private double stDev(List<Action> actions) {
        double stDev = 0;
        for (Action a : actions) {
            stDev += a.variance;
        }
        stDev = stDev / actions.size();
        return Math.sqrt(stDev); //ou of 100 throws, 95 must be below stDev
    }

    public int getId() {
        return id;
    }

    public String getName() {
        return name;
    }
}
