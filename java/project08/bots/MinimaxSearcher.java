package project08.bots;

import edu.caltech.cs2.project08.bots.AbstractSearcher;
import edu.caltech.cs2.project08.bots.BestMove;
import edu.caltech.cs2.project08.game.Board;
import edu.caltech.cs2.project08.game.Evaluator;
import edu.caltech.cs2.project08.game.Move;

public class MinimaxSearcher<B extends Board> extends AbstractSearcher<B> {

    @Override
    public Move getBestMove(B board, int myTime, int opTime) {
        edu.caltech.cs2.project08.bots.BestMove best = minimax(this.evaluator, board, ply);
        return best.move;
    }

    protected static <B extends Board> edu.caltech.cs2.project08.bots.BestMove minimax(Evaluator<B> evaluator, B board, int depth) {

        if (board.isGameOver() || depth == 0) {
            return new edu.caltech.cs2.project08.bots.BestMove(evaluator.eval(board));
        }

        edu.caltech.cs2.project08.bots.BestMove bestMove = new edu.caltech.cs2.project08.bots.BestMove(Integer.MIN_VALUE);

        for (Move move : board.getMoves()) {
            board.makeMove(move);
            int value = -minimax(evaluator, board, depth - 1).score;
            board.undoMove();

            if(value > bestMove.score) {
                bestMove = new BestMove(move, value);
            }
        }

        bestMove.negate();
        return bestMove;
    }
}