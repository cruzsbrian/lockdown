package project08.game;

public interface Timer {
    /**
     * Starts the timer and allocates the appropriate amount of time.
     * You are free to modify this interface.
     *
     * @param myTime amount of time left on your edu.caltech.cs2.game clock.
     * @param opTime amount of time left on your opponent's edu.caltech.cs2.game clock.
     */
    void start(int myTime, int opTime);

    /**
     * Stops the timer and returns the number of milliseconds that have passed since the timer was started.
     *
     * @return millis the timer has been running
     */
    long stop();

    /**
     * @return true if the search should end because the allocated time
     * has been used up.
     */
    boolean timeup();

    /*
     * These methods activate and deactivate the timer. This is useful, for
     * example, if you wanted to make sure that at least a full 1-ply search is
     * completed without interruption, you'd set it to notOk for that search.
     */

    /**
     * Deactivates the timer and will make timeup() always return fales.
     */
    void deactivate();

    /**
     * Reactivates the timer and will make timeup() function normally.
     */
    void activate();
}
