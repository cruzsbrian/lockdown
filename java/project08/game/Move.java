package project08.game;

public class Move {
    public static final int PASS = -1;
    private int square;

    public Move(int sq) {
//        if ((sq < 0 || sq > 63) && sq != PASS) {
//            // Illegal move (invalid square number).
//            // TODO: Add some sort of exception here.
//        }
        this.square = sq;
    }

    public boolean isPass() {
        return square == PASS;
    }

    public int getSquare() {
        return square;
    }

    @Override
    public String toString() {
        if (isPass())
            return "PASS";

        int row = square / 8;
        int col = square % 8;

        return "abcdefgh".charAt(col) + "" + (row + 1);
    }

    @Override
    public boolean equals(Object o) {
        if (o instanceof Move) {
            Move other = (Move) o;
            return other.square == this.square;
        }
        return false;
    }

    @Override
    public int hashCode() {
        return square;
    }
}
