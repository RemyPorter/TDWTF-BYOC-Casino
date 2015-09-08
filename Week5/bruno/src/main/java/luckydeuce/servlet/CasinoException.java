package luckydeuce.servlet;

import luckydeuce.players.User;

public class CasinoException extends RuntimeException {
    private String message;
    private User user;

    public CasinoException(String message, User user) {
        super();
        this.message = message;
        this.user = user;
    }

    @Override
    public String toString() {
        return "CasinoException\n" + "message=" + message + "\n user=" + user + '}';
    }
    
    
}
