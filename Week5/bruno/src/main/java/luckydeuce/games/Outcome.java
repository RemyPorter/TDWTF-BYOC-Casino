package luckydeuce.games;

import java.math.BigDecimal;

class Outcome {
    String newState;
    double amountChange;
    BigDecimal start;
    BigDecimal width;

    Outcome(String newState, double amountChange, BigDecimal start, BigDecimal width) {
        this.newState = newState;
        this.amountChange = amountChange;
        this.start = start;
        this.width = width;
    }
}
