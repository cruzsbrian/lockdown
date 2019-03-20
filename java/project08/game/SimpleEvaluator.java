package project08.game;

import edu.caltech.cs2.project08.game.Board;
import edu.caltech.cs2.project08.game.Evaluator;

public class SimpleEvaluator<B extends Board> implements Evaluator<B> {
    /**
     * A simple parity edu.caltech.cs2.board evaluation relative to the side to move.
     * This works for any class which implements the edu.caltech.cs2.board interface, but
     * your evaluation method doesn't need to.
     *
     * Since the edu.caltech.cs2.board interface defines the score to be positive if black is ahead
     * and negative if white is ahead, we negate it if it isn't black's move.
     *
     * @param board edu.caltech.cs2.board to evaluate
     * @return parity score
     */
    public int eval(B board) {
        return board.getScore() * (board.isBlackMove() ? 1 : -1);
    }
}
