package project08.board;

import edu.caltech.cs2.project08.datastructures.ArrayDeque;
import edu.caltech.cs2.project08.datastructures.LinkedDeque;
import edu.caltech.cs2.project08.game.Board;
import edu.caltech.cs2.project08.interfaces.IDeque;
import edu.caltech.cs2.project08.game.Move;
import edu.caltech.cs2.project08.interfaces.IStack;

public class ArrayBoard implements Board {
    private static final int CORNER_WEIGHT = 10;
    private static final int CORNER_ADJACENT_WEIGHT = -2;
    private static final int MOBILITY_WEIGHT = 1;

    public static final int BLACK = -1;
    public static final int WHITE = -BLACK;
    public static final int EMPTY = 0;

    // Ray offsets, in {row, col} format.
    public static final int[][] OFFSETS = {{-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

    public int[][] board;
    private int sideToMove;
    private IStack<UndoInfo> moves;

    public ArrayBoard() {
        this(START_POS);
    }

    public ArrayBoard(String pos) {
        if (!pos.matches(MOVE_STRING_REGEX)) {
            // Invalid position string (doesn't match regex.)
            // TODO Add some sort of exception here.
        }

        // Load data from the position string into the edu.caltech.cs2.board.
        board = new int[8][8];
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                switch (pos.charAt(r * 8 + c)) {
                    case BLACK_CHAR:
                        board[r][c] = BLACK;
                        break;
                    case WHITE_CHAR:
                        board[r][c] = WHITE;
                        break;
                    case EMPTY_CHAR:
                        board[r][c] = EMPTY;
                        break;
                }
            }
        }

        // Set the side to move.
        sideToMove = pos.charAt(65) == BLACK_CHAR ? BLACK : WHITE;

        // Empty move stack.
        moves = new LinkedDeque<>();
    }

    public void makeMove(Move move) {
        UndoInfo undo = new UndoInfo(move);

        if (move.isPass()) {
            moves.push(undo);
            sideToMove = -sideToMove;
            return;
        }

        int numFlips = 0;

        // Row and col of move.
        int moveRow = move.getSquare() / 8;
        int moveCol = move.getSquare() % 8;
        board[moveRow][moveCol] = sideToMove;

        for (int i = 0; i < OFFSETS.length; i++) {
            int rowOff = OFFSETS[i][0];
            int colOff = OFFSETS[i][1];
            int row = moveRow;
            int col = moveCol;

            // Cast until out-of-bounds, on empty, or on own disk.
            do {
                row += rowOff;
                col += colOff;
            } while (row > -1 && row < 8 && col > -1 && col < 8 && board[row][col] == -sideToMove);

            // Nothing to do in this ray cast direction.
            if (row < 0 || row > 7 || col < 0 || col > 7 || board[row][col] != sideToMove) {
                continue;
            }

            // Undo Information (set the information about this direction).
            undo.flips[i] = new int[]{row, col};

            // "Flip" disks until we get to the start.
            while (row != moveRow || col != moveCol) {
                board[row][col] = sideToMove;
                row -= rowOff;
                col -= colOff;
            }
        }

        moves.push(undo);
        sideToMove = -sideToMove;
    }

    public void undoMove() {
        UndoInfo undo = moves.pop();

        if (undo.move.isPass()) {
            sideToMove = -sideToMove;
            return;
        }

        // Row and col of move.
        int moveRow = undo.move.getSquare() / 8;
        int moveCol = undo.move.getSquare() % 8;

        for (int i = 0; i < OFFSETS.length; i++) {
            if (undo.flips[i] == null) {
                continue;
            }

            // Starting position for ray
            int rowOff = OFFSETS[i][0];
            int colOff = OFFSETS[i][1];
            int row = undo.flips[i][0];
            int col = undo.flips[i][1];

            // Flip the disks until we get back to the start.
            do {
                row -= rowOff;
                col -= colOff;
                board[row][col] = sideToMove;
            } while (row != moveRow || col != moveCol);
        }

        board[moveRow][moveCol] = EMPTY;

        sideToMove = -sideToMove;
    }

    public IDeque<Move> getMoves() {
        IDeque<Move> moves = new ArrayDeque<>(24);

        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                if (board[r][c] != sideToMove) {
                    continue;
                }

                for (int[] offset : OFFSETS) {
                    int rowOff = offset[0];
                    int colOff = offset[1];
                    int row = r;
                    int col = c;
                    int count = 0;

                    // Cast until out-of-bounds, on empty, or on own disk.
                    do {
                        row += rowOff;
                        col += colOff;
                        count++;
                    } while (row > -1 && row < 8 && col > -1 && col < 8 && board[row][col] == -sideToMove);

                    // This row and col is a valid move.
                    if (row > -1 && row < 8 && col > -1 && col < 8 && board[row][col] == EMPTY && count > 1) {
                        moves.add(new Move(row * 8 + col));
                    }
                }
            }
        }

        if (moves.size() == 0) {
            moves.add(new Move(Move.PASS));
        }

        return moves;
    }

    private int[] diskCount() {
        int numBlack = 0;
        int numWhite = 0;
        for (int[] row : board) {
            for (int sq : row) {
                if (sq == BLACK)
                    numBlack++;
                if (sq == WHITE)
                    numWhite++;
            }
        }
        return new int[]{numBlack, numWhite};
    }

    public boolean isGameOver() {
        int[] diskCounts = diskCount();
        int numBlack = diskCounts[0];
        int numWhite = diskCounts[1];
        return numBlack + numWhite == 64 || numBlack == 0 || numWhite == 0;
    }

    public boolean isBlackMove() {
        return sideToMove == BLACK;
    }

    public int getScore() {
        int[] diskCounts = diskCount();
        int numBlack = diskCounts[0];
        int numWhite = diskCounts[1];
        return numBlack - numWhite;
    }

//    private int s(int r, int c) {
//        return
//    }

    public int getNumBlack() {
        return diskCount()[0];
    }

    public int getNumWhite() {
        return diskCount()[1];
    }

    private int getMobility() {
        return 10 * getMoves().size();
    }

    private int getWeightedSquares() {
        return  (int) (
                getScore() +
                -CORNER_WEIGHT * (board[0][0] + board[0][7] + board[7][0] + board[7][7]) +
                -CORNER_ADJACENT_WEIGHT * (
                        board[1][0] + board[0][1] + board[1][1] + board[0][6] + board[1][7] + board[1][6] +
                        board[7][1] + board[6][0] + board[6][1] + board[6][7] + board[7][6] + board[6][6]) +
                MOBILITY_WEIGHT * (getMobility())
        );
    }

    private int getStability() {
        return 0;
    }

    public int getWeightedValues() { // black = +score, white = -score
        return getMobility() + getWeightedSquares() + getStability();
    }

    public String toString() {
        String ret = "  ┌─────────────────┐\n";
        int r = 1;
        for (int[] row : board) {
            ret += r + " │";
            for (int sq : row) {
                ret += ' ';
                switch (sq) {
                    case BLACK:
                        ret += BLACK_CHAR;
                        break;
                    case WHITE:
                        ret += WHITE_CHAR;
                        break;
                    case EMPTY:
                        ret += EMPTY_CHAR;
                }
            }
            ret += " │\n";
            r++;
        }
        ret += "  └─────────────────┘\n";
        ret += "    A B C D E F G H\n";
        return ret;
    }

    public String posString() {
        String pos = "";
        for (int[] row : board) {
            for (int sq : row) {
                switch (sq) {
                    case BLACK:
                        pos += BLACK_CHAR;
                        break;
                    case WHITE:
                        pos += WHITE_CHAR;
                        break;
                    default:
                        pos += EMPTY_CHAR;
                        break;
                }
            }
        }
        pos += " " + (sideToMove == BLACK ? BLACK_CHAR : WHITE_CHAR);
        return pos;
    }

    private class UndoInfo {
        public int[][] flips;
        public Move move;

        public UndoInfo(Move move) {
            this.flips = new int[OFFSETS.length][];
            this.move = move;
        }
    }

    public ArrayBoard copy() {
        return new ArrayBoard(this.posString());
    }
}
