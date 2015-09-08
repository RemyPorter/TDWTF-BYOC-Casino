package luckydeuce.games;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import luckydeuce.AppSingleton;
import luckydeuce.random.RandomProvider;
import luckydeuce.random.ReplayebleSecureRandomProviderImpl;

public class GameHistory {

    private Long[] seed;
    private List[] actions;

    public GameHistory() {
        int gamesCount = AppSingleton.instance().getGameFactory().getGamesCount();
        seed = new Long[gamesCount];
        actions = new List[gamesCount];
    }

    boolean hasPlayed(int game) {
        return seed[game] != null && actions[game] != null;
    }

    public void switchGame(RandomProvider random, int game) {
        if (seed[game] == null) {
            seed[game] = System.currentTimeMillis();
            actions[game] = new ArrayList<String>();
            random.randomize(seed[game], 0);
        } else {
            random.randomize(seed[game], actions[game].size());
        }
    }

    boolean replay(int id) {
        if (actions[id] == null || actions[id].size() < 25) {
            return false;
        }
        double mean = 0;
        double sqrd = 0;
        
        for (int i = 0; i < AppSingleton.MONTE_CARLO_LIMIT; i++) {
            double payout = randomPlay(id, actions[id].size());
            mean += payout;
            sqrd += payout * payout;
        }
        double currentPayout = replay(id, actions[id], new ReplayebleSecureRandomProviderImpl(seed[id], 0));
        double stDevPop = sqrd - (mean / AppSingleton.MONTE_CARLO_LIMIT);
        stDevPop = Math.sqrt(stDevPop / (AppSingleton.MONTE_CARLO_LIMIT - 1));
        mean = mean / AppSingleton.MONTE_CARLO_LIMIT;
        return currentPayout >= (mean + 3 * stDevPop);
    }

    double randomPlay(int id, int rounds) {
        RandomProvider random = new ReplayebleSecureRandomProviderImpl();
        Game game = AppSingleton.instance().getGameFactory().create(id);
        double payout = 0;
        for (int i = 0; i < rounds; i++) {
            Action action = game.find(game.getActions().get(random.nextRandom() % game.getActions().size()));
            payout += game.resolve(action, random.nextRandom());
        }
        return payout;
    }

    double replay(int id, List<String> actions, RandomProvider random) {
        Game game = AppSingleton.instance().getGameFactory().create(id);
        double payout = 0;
        for (String a : actions) {
            Action action = game.find(a);
            payout += game.resolve(action, random.nextRandom());
        }
        return payout;
    }
}
