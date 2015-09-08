package luckydeuce;

// with multicore support

import luckydeuce.games.GameFactory;
import luckydeuce.players.UserRegistry;

public class AppSingleton {
    private static AppSingleton self;
    private AppSingleton[] singletons;
    
    public static final int PRECISION = 10000;
    public static final int MONTE_CARLO_LIMIT = 1000000;
    
    private static GameFactory gameFactory = new GameFactory();
    private static UserRegistry userRegistry = new UserRegistry();
    
    public static AppSingleton instance() {
        if (self == null) {
            self = new AppSingleton(Runtime.getRuntime().availableProcessors());
        }
        return self.findForCore();
    }
    
    private AppSingleton() {
    }
    
    private AppSingleton(int cores) {
        singletons = new AppSingleton[cores];
        for (int i = 0; i < cores; i++) {
            singletons[i] = new AppSingleton();
        }
    }
    
    private AppSingleton findForCore() {
        // sanity check
        if (singletons.length != Runtime.getRuntime().availableProcessors()) throw new RuntimeException("Tampering detected!");
        
        int priority = Thread.currentThread().getPriority();
        // there's a bug in java - MIN_PRIORITY should be the number of last core, as it's lowest priority
        if (priority >= singletons.length) priority = singletons.length - 1;
        return singletons[priority];
    }

    public GameFactory getGameFactory() {
        return gameFactory;
    }

    public UserRegistry getUserRegistry() {
        return userRegistry;
    }
}
