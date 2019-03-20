package play;

import edu.caltech.cs2.project08.board.ArrayBoard;
import edu.caltech.cs2.project08.board.ArrayBoardFactory;
import edu.caltech.cs2.project08.board.BitBoard;
import edu.caltech.cs2.project08.board.BitBoardFactory;
import edu.caltech.cs2.project08.bots.AbstractSearcher;
import edu.caltech.cs2.project08.bots.AlphaBetaSearcher;
import edu.caltech.cs2.project08.bots.MinimaxSearcher;
import edu.caltech.cs2.project08.game.*;

public class Bot {

    public static final String BOT_NAME = "OutstandingMove";
    public static final String BOT_PASS = "FvSyQwSmpTMK";

//    public static final String BOT_NAME = "OutstandingMove2";
//    public static final String BOT_PASS = "zHQcvyZPtBUL";

//    public static final String BOT_NAME = "OutstandingMove3";
//    public static final String BOT_PASS = "jNCSxwzWbHET";

    public static final int DEPTH = 8;

    public class BasicBot extends Bot {
        private Evaluator<ArrayBoard> evaluator;
        private BoardFactory<ArrayBoard> boardFactory;
        private AbstractSearcher<ArrayBoard> searcher;

        public BasicBot() {
            // TODO: Set up your evaluator and searcher here.
            evaluator = new SimpleEvaluator<>();
            boardFactory = new ArrayBoardFactory();

            // Make sure you set all of the necessary attributes on your searcher!
            searcher = new AlphaBetaSearcher<>();
            searcher.setDepth(DEPTH);
            searcher.setEvaluator(evaluator);
        }

        public Move getBestMove(String position, int myTime, int opTime) {
            boardFactory.setPosition(position);
            ArrayBoard board = boardFactory.create();
            return searcher.getBestMove(board, myTime, opTime);
        }
    }

    public class BitBoardBot extends Bot {
        private Evaluator<BitBoard> evaluator;
        private BoardFactory<BitBoard> boardFactory;
        private AbstractSearcher<BitBoard> searcher;

        public BitBoardBot() {
            // TODO: Set up your evaluator and searcher here.
            evaluator = new WeightedEvaluator<>();
            boardFactory = new BitBoardFactory();

            // Make sure you set all of the necessary attributes on your searcher!
            searcher = new AlphaBetaSearcher<>();
            searcher.setDepth(DEPTH);
            searcher.setEvaluator(evaluator);
        }

        public Move getBestMove(String position, int myTime, int opTime) {
            boardFactory.setPosition(position);
            BitBoard board = boardFactory.create();
            return searcher.getBestMove(board, myTime, opTime);
        }
    }

    private Evaluator<BitBoard> evaluator;
    private BoardFactory<BitBoard> boardFactory;
    private AbstractSearcher<BitBoard> searcher;

    public Bot() {
        // TODO: Set up your evaluator and searcher here.
        evaluator = new WeightedEvaluator<>();
        boardFactory = new BitBoardFactory();

        // Make sure you set all of the necessary attributes on your searcher!
        searcher = new AlphaBetaSearcher<>();
        searcher.setDepth(DEPTH);
        searcher.setEvaluator(evaluator);
    }

    public Move getBestMove(String position, int myTime, int opTime) {
        boardFactory.setPosition(position);
        BitBoard board = boardFactory.create();
        return searcher.getBestMove(board, myTime, opTime);
    }

}
