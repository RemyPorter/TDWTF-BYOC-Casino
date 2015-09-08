package luckydeuce.games;

import java.math.BigDecimal;

class GameFileData {
    String state;
    String action;
    String newState;
    double amountChange;
    BigDecimal start;
    BigDecimal width;
    
    GameFileData() {}

    GameFileData(String state, String action, String newState, double amountChange, BigDecimal start, BigDecimal width) {
        this.state = state;
        this.action = action;
        this.newState = newState;
        this.amountChange = amountChange;
        this.start = start;
        this.width = width;
    }
}
