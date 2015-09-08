package luckydeuce.random;

import luckydeuce.players.User;

public class RandomTest {
    public static void main(String[] args) {
        ReplayebleSecureRandomProviderImpl random = new ReplayebleSecureRandomProviderImpl();
        User u1 = new User("a", "b");
        //u1.nextSavedSecureRandom();
        //u1.nextSavedSecureRandom();
        u1.nextRepeatableSecureRandom();
        u1.nextRepeatableSecureRandom();
        User u2 = new User(u1);
        for (int i = 0; i < 10; i++) {
            System.out.println(u1.nextSavedSecureRandom());
            System.out.println(u2.nextSavedSecureRandom());
        }
    }
}
