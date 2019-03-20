package project08.board;

import edu.caltech.cs2.project08.board.ArrayBoard;
import edu.caltech.cs2.project08.game.BoardFactory;

public class ArrayBoardFactory implements BoardFactory<edu.caltech.cs2.project08.board.ArrayBoard> {
    private String pos;

    public void setPosition(String pos) {
        this.pos = pos;
    }

    public edu.caltech.cs2.project08.board.ArrayBoard create() {
        return new ArrayBoard(pos);
    }
}
