//package edu.caltech.cs2.project08.bots;
//
//import edu.caltech.cs2.project08.datastructures.ChainingHashDictionary;
//import edu.caltech.cs2.project08.game.Board;
//import edu.caltech.cs2.project08.game.Evaluator;
//import edu.caltech.cs2.project08.game.Move;
//import edu.caltech.cs2.project08.interfaces.IDeque;
//import edu.caltech.cs2.project08.interfaces.IDictionary;
//
//import java.util.Dictionary;
//import java.util.Hashtable;
//
//public class NegaScoutSearcher<B extends Board> extends AbstractSearcher<B> {
//
//    class Tuple {
//        public Tuple(int depth, Board board) {
//            this.depth = depth;
//            this.board = board;
//        }
//        int depth;
//        Board board;
//    }
//
//    static final double PERCENTAGE_SEQUENTIAL = 0.5;
//    Dictionary<Tuple, BestMove> hashtable = new Hashtable<>();
//
//    class AlphaBetaThread implements Runnable {
//        BestMove result = null;
//
//        public void run() {
//
//        }
//    }
//
//    @Override
//    public Move getBestMove(B board, int myTime, int opTime) {
//
//        BestMove best = negaScout(this.evaluator, board, ply, Integer.MIN_VALUE, Integer.MAX_VALUE);
////        best.negate();
//        return best.move;
//    }
//
//    private <B extends Board> BestMove negaScout(Evaluator<B> evaluator, B board, int depth, int alpha, int beta) {
//
//        Tuple boardInfo = new Tuple(depth, board);
//        BestMove hashTableResult = hashtable.get(boardInfo);
//        if (hashTableResult != null) return hashTableResult;
//
//        if (board.isGameOver() || depth == 0) {
//            BestMove move = new BestMove(evaluator.eval(board));
//            hashtable.put(boardInfo, move);
//            return move;
//        }
//
//        IDeque<Move> moves = board.getMoves();
//
//        int length = moves.size();
//
//        for(int i = 0; i < Math.floor(PERCENTAGE_SEQUENTIAL*length); ++i) {
//            Move move = moves.removeBack();
//            board.makeMove(move);
//            BestMove m = jamboree(evaluator, board, depth - 1, -beta, -alpha);
//            int value = -m.score;
//
//            board.undoMove();
//
//            if(value > alpha) {
//                alpha = value;
//            }
//            if(alpha >= beta) {
//                return new BestMove(move, alpha);
//            }
//        }
//
//        for (double i = Math.ceil(PERCENTAGE_SEQUENTIAL*length); i < length; ++i) {
//            Move move = moves.removeBack();
//            board.makeMove(move);
//            Board newboard = board.copy();
//            BestMove m = jamboree(evaluator, board, depth - 1, -beta, -alpha);
//            int value = -m.score;
//
//            if(value > alpha) {
//                alpha = value;
//            }
//            if(alpha >= beta) {
//                return new BestMove(m.move, alpha);
//            }
//        }
//
//        return new BestMove(alpha);
//    }
//}