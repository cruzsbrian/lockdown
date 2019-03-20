package project08.board;

import edu.caltech.cs2.project08.board.BitBoard;
import edu.caltech.cs2.project08.game.BoardFactory;

public class BitBoardFactory implements BoardFactory<edu.caltech.cs2.project08.board.BitBoard> {
    private String pos;

    public void setPosition(String pos) {
        this.pos = pos;
    }

    public edu.caltech.cs2.project08.board.BitBoard create() {
        return new BitBoard(pos);
    }
}
