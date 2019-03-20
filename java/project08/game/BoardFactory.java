package project08.game;

import edu.caltech.cs2.project08.game.Board;

public interface BoardFactory<B extends Board> {
    /**
     * Set the position to create the edu.caltech.cs2.board from.
     * @param pos position
     */
    void setPosition(String pos);

    /**
     * Returns a edu.caltech.cs2.board which has the position specified by the setPosition method.
     * @return a new edu.caltech.cs2.board in the specified position
     */
    B create();
}
