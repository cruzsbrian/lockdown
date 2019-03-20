package project08.bots;

import edu.caltech.cs2.project08.bots.AbstractSearcher;
import edu.caltech.cs2.project08.bots.BestMove;
import edu.caltech.cs2.project08.game.Board;
import edu.caltech.cs2.project08.game.Evaluator;
import edu.caltech.cs2.project08.game.Move;
import edu.caltech.cs2.project08.interfaces.IDeque;

public class AlphaBetaSearcher<B extends Board> extends AbstractSearcher<B> {

    static final double PERCENTAGE_SEQUENTIAL = 0.5;

    class AlphaBetaThread<B extends Board> extends Thread {
        edu.caltech.cs2.project08.bots.BestMove result = null;
        Evaluator<B> evaluator;
        protected B board;
        int depth, alpha, beta;
        boolean running = false;

        public AlphaBetaThread(Evaluator<B> evaluator, B board, int depth, int alpha, int beta) {
            this.evaluator = evaluator;
            this.board = board;
            this.depth = depth;
            this.alpha = alpha;
            this.beta = beta;
        }

        public void run() {
            this.running = true;
            this.result = jamboree(evaluator, board, depth, alpha, beta);
            this.running = false;
        }
    }

    @Override
    public Move getBestMove(B board, int myTime, int opTime) {

        edu.caltech.cs2.project08.bots.BestMove best = jamboree(this.evaluator, board, ply, -Integer.MAX_VALUE, Integer.MAX_VALUE);
        return best.move;
    }

    private static <B extends Board> edu.caltech.cs2.project08.bots.BestMove alphaBeta(Evaluator<B> evaluator, B board, int depth, int alpha, int beta) {
        if (board.isGameOver() || depth == 0) {
            return new edu.caltech.cs2.project08.bots.BestMove(evaluator.eval(board));
        }
        IDeque<Move> moves = board.getMoves();

        edu.caltech.cs2.project08.bots.BestMove bestMove = new edu.caltech.cs2.project08.bots.BestMove(null, -Integer.MAX_VALUE);

        for (Move move: moves) {
            board.makeMove(move);
            int value = -alphaBeta(evaluator, board, depth - 1, -beta, -alpha).score;
            board.undoMove();

            if (value > bestMove.score) {
                bestMove.score = value;
                bestMove.move = move;
            }

            if (bestMove.score > alpha) {
                alpha = bestMove.score;
            }

            if (bestMove.score >= beta) {
                break;
            }
        }

        return bestMove;
    }

    private <B extends Board> edu.caltech.cs2.project08.bots.BestMove jamboree(Evaluator<B> evaluator, B board, int depth, int alpha, int beta) {
        if (board.isGameOver() || depth == 0) {
            return new edu.caltech.cs2.project08.bots.BestMove(evaluator.eval(board));
        }
        IDeque<Move> moves = board.getMoves();

        edu.caltech.cs2.project08.bots.BestMove bestMove = new BestMove(null, -Integer.MAX_VALUE);

        int length = moves.size();
        int cutoff = (int) Math.floor(PERCENTAGE_SEQUENTIAL*length);

        for (int i = 0; i < cutoff; ++i) {
            Move move = moves.removeBack();
            board.makeMove(move);
            int value = -alphaBeta(evaluator, board, depth - 1, -beta, -alpha).score;
            board.undoMove();

            if (value > bestMove.score) {
                bestMove.score = value;
                bestMove.move = move;
            }

            if (bestMove.score > alpha) {
                alpha = bestMove.score;
            }

            if (bestMove.score >= beta) {
                break;
            }
        }

        AlphaBetaThread[] threads = new AlphaBetaThread[length - cutoff];
        boolean running = true;

        for(int i = 0; i < length - cutoff; ++i) {
            Board bcopy = board.copy();
            AlphaBetaThread newThread = new AlphaBetaThread(evaluator, bcopy, depth - 1, -beta, -alpha);
            threads[i] = newThread;
            bcopy.makeMove(moves.removeBack());
            newThread.run();
        }

        while (running) {
            running = false;
            for (AlphaBetaThread thread : threads) {
                if (thread.running) running = true;
            }
        }

        for (AlphaBetaThread thread : threads) {
            if (-thread.result.score > bestMove.score) {
                bestMove = thread.result;
            }
        }

        return bestMove;
    }


}