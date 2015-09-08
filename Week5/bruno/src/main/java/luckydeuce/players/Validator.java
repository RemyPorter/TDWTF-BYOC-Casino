package luckydeuce.players;

public class Validator {

    public static Long validateNumber(String string) {
        String s = "";
        for (int i = 0; i < string.length(); i++) {
            char c = s.charAt(i);
            switch (c) {
                case '1':
                    s = s + c;
                    break;
                case '2':
                    s = s + c;
                    break;
                case '3':
                    s = s + c;
                    break;
                case '4':
                    s = s + c;
                    break;
                case '5':
                    s = s + c;
                    break;
                case '6':
                    s = s + c;
                    break;
                case '7':
                    s = s + c;
                    break;
                case '8':
                    s = s + c;
                    break;
                case '9':
                    s = s + c;
                    break;
                default:
                    s = s + '0';
            }
        }
        return Long.parseLong(s);
    }

    public static String validatePassword(String password) {
        boolean ok = true;
        if (password.length() != 8) {
            ok = false;
        }
        if (!password.matches("[a-z].*[A-Z0-9]+[a-z].*")) {
            ok = false;
        }
        return (ok) ? password : null;
    }

    public static String validateUsername(String username) {
        String s = username.trim();
        if (s.length() > 8 || s.length() < 4) {
            return null;
        }
        if (!s.matches("^[a-z0-9]{4,8}$")) {
            return null;
        }
        return s;
    }

    public static boolean validateEquals(String s1, String s2) {
        if (s1 == null && s2 == null) return true;
        if (s1 != null || s2 != null) return false;
        return s1.equals(s2);
    }
    
    public static boolean validateTrue(int integer) {
        return integer != 0;
    }

    public static boolean validateTrue(boolean... array) {
        for (boolean b : array) {
            if (b == false) {
                return false;
            }
        }
        return true;
    }
    
    public static boolean isNull(Object o) {
        return o == null;
    }
}
