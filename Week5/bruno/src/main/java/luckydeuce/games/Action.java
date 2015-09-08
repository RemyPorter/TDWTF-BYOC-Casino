package luckydeuce.games;

import java.math.BigDecimal;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import luckydeuce.AppSingleton;

class Action {
    List<Outcome> outcomes = new ArrayList<Outcome>();
    String name;
    double expectedPayout;
    double variance;
    
    Action(String name) {
        this.name = name;
    }
    
    void addOutcome(Outcome o) {
        outcomes.add(o);
        calculateExpectedPayout();
    }
    
    Outcome resolve(Integer random) {
        Outcome outcome = null;
        BigDecimal chance = convertRandom(random);
        Iterator<Outcome> iter = outcomes.iterator();
        while (iter.hasNext()) {
            outcome = iter.next();
            if (chance.compareTo(outcome.start) * chance.compareTo(outcome.start.add(outcome.width)) < 1) {
                return outcome;
            }
        }
        // we will never get here
        throw new RuntimeException();
    }
    
    boolean isBetValid(double playerAmount) {
        for (Outcome o : outcomes) {
            if (playerAmount + o.amountChange < 0) return false;
        }
        return true;
    }
    
    private void calculateExpectedPayout() {
        expectedPayout = 0.;
        for (Outcome o : outcomes) {
            expectedPayout += o.width.doubleValue() * o.amountChange;
        }
        variance = 0.;
        for (Outcome o : outcomes) {
            variance += (o.amountChange - expectedPayout) * (o.amountChange - expectedPayout) * o.width.doubleValue();
        }
    }
    
    private BigDecimal convertRandom(Integer random) {
        double val = (random.doubleValue() % AppSingleton.PRECISION) / AppSingleton.PRECISION;
        return new BigDecimal(val);
    }
}
