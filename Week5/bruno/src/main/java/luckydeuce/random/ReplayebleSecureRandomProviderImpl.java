package luckydeuce.random;

import java.util.Random;

public class ReplayebleSecureRandomProviderImpl implements RandomProvider {
    protected Random random = new Random();
    protected long seed;
    protected int iterations;
    
    public ReplayebleSecureRandomProviderImpl() {
        this.seed = System.currentTimeMillis();
        random.setSeed(seed);
    }
    
    public ReplayebleSecureRandomProviderImpl(long seed, int iterations) {
        this.seed = seed;
        this.iterations = iterations;
        random.setSeed(seed);
        for (int i = 0; i < iterations; i++) random.nextInt();
    }
    
    public ReplayebleSecureRandomProviderImpl(ReplayebleSecureRandomProviderImpl random) {
        this(random.seed, random.iterations);
    }

    public Integer nextRandom() {
        iterations++;
        Integer r = random.nextInt(Integer.MAX_VALUE);
        System.out.println("S:" + seed + ";I:"+iterations + "R:" + r);
        return r;
    }

    public void randomize(Long seed, int iterations) {
        this.seed = seed;
        iterations = 0;
        random.setSeed(seed);
        for (int i = 0; i < iterations; i++) random.nextInt();
    }

    @Override
    public String toString() {
        return "ReplayebleSecureRandomProviderImpl{" + "random=" + random + ", seed=" + seed + ", iterations=" + iterations + '}';
    }
}
