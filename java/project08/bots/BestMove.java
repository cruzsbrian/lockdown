package project08.bots;

import edu.caltech.cs2.project08.game.Move;

public class BestMove {
    public Move move;
    public int score;

    public BestMove(int score) {
        this.score = score;
    }

    public BestMove(Move move, int score) {
        this.move = move;
        this.score = score;
    }

    public void negate() {
        this.score = -this.score;
    }
}
