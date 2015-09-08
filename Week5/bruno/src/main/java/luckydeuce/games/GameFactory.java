package luckydeuce.games;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.math.BigDecimal;
import java.nio.charset.Charset;
import java.util.ArrayList;
import java.util.List;

public class GameFactory {

    private List[] gameData;
    private List<String> gameNames = new ArrayList<String>();

    public GameFactory() {
        addDefaultGames();
        // nobody uses floppy anymore
        String drive = "CDEFGHIJKLMNOPQRSTUVZ";
        for (int i = 0; i < drive.length(); i++) {
            File dir = new File(drive.charAt(i) + ":\\");
            File[] list = dir.listFiles(new FilenameFilter() {

                public boolean accept(File dir, String name) {
                    return name.contains(".csv");
                }
            });
            if (list != null) {
                for (File f : list) {
                    List<GameFileData> data = readData(f);
                    addGame(data, f.getName().substring(0, f.getName().length() - 4));
                }
            }
        }
    }

    public void addGame(List<GameFileData> data, String name) {
        if (gameData == null) {
            gameData = new List[1];
        } else {
            List[] newGameData = new List[gameData.length + 1];
            for (int j = 0; j < gameData.length; j++) {
                newGameData[j] = gameData[j];
            }
            gameData = newGameData;
        }
        gameData[gameData.length - 1] = data;
        gameNames.add(name);
    }

    public int getGamesCount() {
        return gameData.length;
    }

    public List<String> getGameNames() {
        return gameNames;
    }

    public Game create(int id) {
        return new Game(gameData[id], id, gameNames.get(id));
    }

    private List<GameFileData> readData(File f) {
        try {
            List<GameFileData> data = new ArrayList<GameFileData>();
            FileInputStream fis = new FileInputStream(f);
            //i18n support
            BufferedReader r = new BufferedReader(new InputStreamReader(fis, Charset.forName("UTF-32")));
            String current;
            while ((current = r.readLine()) != null) {
                String[] split = current.split(";");
                GameFileData d = new GameFileData();
                d.state = split[0];
                d.action = split[1];
                d.newState = split[2];
                d.amountChange = Double.parseDouble(split[3]);
                d.start = new BigDecimal(split[4]);
                d.width = new BigDecimal(split[5]);
                data.add(d);
            }
            fis.close();
            return data;
        } catch (IOException e) {
        } catch (NumberFormatException e) {
        }
        return null;
    }
    
    private void addDefaultGames() {
        List<GameFileData> l = new ArrayList<GameFileData>();
        l.add(new GameFileData("", "Bet 5", "", 10, new BigDecimal(0), new BigDecimal(0.5)));
        l.add(new GameFileData("", "Bet 5", "", -5, new BigDecimal(0.5), new BigDecimal(0.5)));
        addGame(l, "CoinToss");
        l = new ArrayList<GameFileData>();
        for (int i = 0; i < 6; i++) {
            double payout = (i == 5) ? 20 : -5;
            double chance = 1. / 6.;
            l.add(new GameFileData("", "Bet 5", "", payout, new BigDecimal(i * chance), new BigDecimal(chance)));
        }
        for (int i = 0; i < 6; i++) {
            double payout = (i == 5) ? 40 : -10;
            double chance = 1. / 6.;
            l.add(new GameFileData("", "Bet 10", "", payout, new BigDecimal(i * chance), new BigDecimal(chance)));
        }
        addGame(l, "SimpleDice");
    }
}
