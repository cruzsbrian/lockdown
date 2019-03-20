package project08.game;

import edu.caltech.cs2.project08.board.BitBoard;
import edu.caltech.cs2.project08.game.Board;
import edu.caltech.cs2.project08.game.Evaluator;

public class WeightedEvaluator<B extends Board> implements Evaluator<B> {
    /**
     * A simple parity edu.caltech.cs2.board evaluation relative to the side to move.
     * This works for any class which implements the edu.caltech.cs2.board interface, but
     * your evaluation method doesn't need to.
     *
     * Since the edu.caltech.cs2.board interface defines the score to be positive if black is ahead
     * and negative if white is ahead, we negate it if it isn't black's move.
     *
     * This function uses the "getWeightedValues" method now part of the Board interface to return
     * a more accurate representation of the board state utility
     */
    public int eval(B board) {
        return board.getWeightedValues() * (board.isBlackMove() ? 1 : -1);
    }
}
