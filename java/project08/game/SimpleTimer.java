package project08.game;

import edu.caltech.cs2.project08.game.Timer;

public class SimpleTimer implements Timer {
    @SuppressWarnings("unused")
    private int initialTime; // TODO: A proper timer should use this information.
    private long startTime;
    private int allocated;

    private boolean enabled = true;

    public SimpleTimer(int time) {
        this.initialTime = time;
    }

    public void start(int myTime, int opTime) {
        startTime = System.currentTimeMillis();
        allocated = myTime / 30;
    }

    public long stop() {
        return System.currentTimeMillis() - startTime;
    }

    public boolean timeup() {
        return enabled && (System.currentTimeMillis() - startTime) > allocated;
    }

    public void deactivate() {
        enabled = false;
    }

    public void activate() {
        enabled = true;
    }

}
