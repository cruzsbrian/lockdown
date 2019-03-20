package play;

import edu.caltech.cs2.project08.game.Move;
import io.socket.client.IO;
import io.socket.client.Socket;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.*;
import java.util.*;

public class Play {
    public static final String SERVER = "http://othello.countablethoughts.com/";
    private static boolean WAITING = false;

    public static JSONArray bots;
    public static List<String> games;
    public static boolean isConnected = false;

    public static void main(String[] args) {
        games = new ArrayList<>();
        Bot bot = new Bot();
        try {
            IO.Options opts = new IO.Options();
            opts.reconnection = true;
            opts.reconnectionDelay = 1000;
            opts.timeout = -1;

            Socket socket = IO.socket(SERVER + "?bot=" + Bot.BOT_NAME + "&pw=" + Bot.BOT_PASS, opts);
            socket.on(Socket.EVENT_CONNECT, (arg) -> {
                //System.out.println("Connected to server.");
                isConnected = true;
            }).on(Socket.EVENT_DISCONNECT, (arg) -> {
                //System.out.println("Disconnected from server.");
            }).on("bad_connect", (arg) -> {
                try {
                    System.out.println(((JSONObject) arg[1]).getString("message"));
                } catch (JSONException e) { }
                System.exit(1);
            })/*.on("gameover", (arg) -> {
                JSONObject obj = (JSONObject) arg[0];
                try {
                    String result = obj.getString("result");
                    if (result.equals("draw")) {
                        System.out.println("The game was a draw!");
                    }
                    else {
                        System.out.println(result + " won!");
                    }
                } catch (JSONException e) {
                    e.printStackTrace();
                }
                if (WAITING) {
                    System.out.print(">> ");
                    WAITING = false;
                }
            })*/.on("failure", (arg) -> {
                JSONObject obj = (JSONObject) arg[0];
                String message = "";
                try {
                    message = (String) obj.get("message");
                } catch (JSONException e) { }
                System.out.println(message);
                if (WAITING) {
                    System.out.print(">> ");
                    WAITING = false;
                }

            }).on("sv_ping", (arg) -> {
                socket.emit("cl_pong");
            }).on("match", (arg) -> {
                JSONObject obj = (JSONObject) arg[0];
                String pos = "";
                int myTime = 0, opTime = 0;
                String gameId = "";
                try {
                    pos = obj.getString("pos");
                    myTime = obj.getInt("my_time");
                    opTime = obj.getInt("op_time");
                    gameId = obj.getString("game_id");
                } catch (JSONException e) { }

                if (!games.contains(gameId)) {
                    System.out.println("Watch the game at: " + SERVER + "watch?game=" + gameId);
                    games.add(gameId);
                }

                if (WAITING) {
                    System.out.print(">> ");
                    WAITING = false;
                }

                // Spawn a new thread so we don't timeout and disconnect.
                String finalPos = pos;
                int finalMyTime = myTime;
                int finalOpTime = opTime;
                String finalId = gameId;
                Thread thread = new Thread(() -> {
                    Move best = bot.getBestMove(finalPos, finalMyTime, finalOpTime);
                    socket.emit("move", best, finalId);
                });
                thread.start();
            }).on("info", (arg) -> {
                JSONObject obj = (JSONObject) (arg.length == 1 ? arg[0] : arg[1]);
                try {
                    bots = obj.getJSONArray("bots");
                } catch (JSONException e) { }
            });
            socket.connect();

            while (!isConnected) {
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) { }
            }

            Scanner scanner = new Scanner(System.in);
            System.out.println("Press enter to start! Commands:");
            System.out.println("\t match <bot name> -- challenge bot to a game");
            System.out.println("\t who -- list bots available to match");
            System.out.println("\t scores -- display scores of previous games");
            System.out.print(">> ");
            String line = scanner.nextLine();
            do {
                String[] parts = line.split(" ");
                if (parts[0].equals("match")) {
                    if (parts.length == 1) {
                        System.out.println("You must specify who you want to match!");
                        System.out.print(">> ");
                        continue;
                    }
                    socket.emit("challenge", parts[1]);
                    WAITING = true;
                } else if (parts[0].equals("scores")) {
                    printResults();
                } else if (parts[0].equals("who")) {
                    printBots();
                }
                if (!WAITING) {
                    System.out.print(">> ");
                }
            } while ((line = scanner.nextLine()) != null);
            scanner.close();
        } catch (URISyntaxException e) {
            System.err.println("Error encountered with SocketIO URL, check SERVER.");
        }
    }

    public static void printBots() {
        System.out.println("Online Bots:");
        if (bots == null) {
            return;
        }

        for (int i = 0; i < bots.length(); i++) {
            try {
                JSONObject botObj = bots.getJSONObject(i);
                String name = botObj.getString("name");
                String status = botObj.getString("status");
                System.out.printf("\t%s - %s%s\n", name, status, name.equals(Bot.BOT_NAME) ? " - You" : "");
            } catch (JSONException e) { }
        }
    }

    public static void printResults() {
        try {
            URL url = new URL(SERVER + "results?bot=" + Bot.BOT_NAME);
            HttpURLConnection con = (HttpURLConnection) url.openConnection();
            con.setRequestMethod("GET");
            con.connect();

            BufferedReader in = new BufferedReader(new InputStreamReader(con.getInputStream()));
            String inputLine;
            StringBuffer content = new StringBuffer();
            while ((inputLine = in.readLine()) != null) {
                content.append(inputLine);
            }
            in.close();

            System.out.println("Results:");

            Map<String, String> scores = new TreeMap<>();

            JSONArray arr = new JSONArray(content.toString());
            for (int i = 0; i < arr.length(); i++) {
                JSONObject res = arr.getJSONObject(i);
                String result = res.getString("result");
                String against = res.getString("against");
                if (!scores.containsKey(against)) {
                    scores.put(against, "");
                }
                scores.put(against, scores.get(against) + result.charAt(0) + " ");
            }

            int l = 0;
            for (String k : scores.keySet()) {
                l = Math.max(l, k.length());
            }

            for (String k : scores.keySet()) {
                String result = scores.get(k);
                int len = Math.max(0, result.length() - 20);
                System.out.println("    " + k + ": " + " ".repeat(l - k.length() + 1) + result.substring(len));
            }
        } catch (JSONException e) {
            System.out.println("Error parsing JSON data.");
        } catch (Exception e) {
            System.out.println("Error retrieving scores.");
        }
    }
}
