package luckydeuce.random;

public interface RandomProvider {
    Integer nextRandom();
    void randomize(Long seed, int step);
}
