package project08.board;

import edu.caltech.cs2.project08.board.BitBoard_precomp;
import edu.caltech.cs2.project08.datastructures.ArrayDeque;
import edu.caltech.cs2.project08.game.Board;
import edu.caltech.cs2.project08.interfaces.IDeque;
import edu.caltech.cs2.project08.game.Move;

import static java.lang.Long.rotateRight;

public class BitBoard implements Board {

    private static final int CORNER_WEIGHT = 10;
    private static final int CORNER_ADJACENT_WEIGHT = -2;

    public static final boolean BLACK = true;
    public static final boolean WHITE = false;

    private final long[] dirs = edu.caltech.cs2.project08.board.BitBoard_precomp.dirs;
    public final long[] n_vals = edu.caltech.cs2.project08.board.BitBoard_precomp.n_vals;
    private final int[] RSHIFTS = edu.caltech.cs2.project08.board.BitBoard_precomp.RSHIFTS;
    private final int[] LSHIFTS = BitBoard_precomp.LSHIFTS;

    // 8 bitboards: white/black
    private long board_w_0;
    private long board_b_0;
    private long board_w_45r;
    private long board_b_45r;
    private long board_w_45l;
    private long board_b_45l;
    private long board_w_90r;
    private long board_b_90r;

    private boolean sideToMove;
    private ArrayDeque<UndoInfo> moves = new ArrayDeque<>(60);
    private static byte moves_remaining = 60;

    public BitBoard() {
        this(START_POS);
    }

    public BitBoard(String pos) {
        this.board_w_0 = 0L;
        this.board_b_0 = 0L;

        for (int i = 0; i < 64; ++i) {
            char c = pos.charAt(i);
            if (c == BLACK_CHAR) {
                this.board_b_0 |= n_vals[i];
            }
            else if (c == WHITE_CHAR) {
                this.board_w_0 |= n_vals[i];
            }
        }

        this.sideToMove = pos.charAt(65)==BLACK_CHAR ? BLACK: WHITE;

//        updateBoardsFromNormalized();

//        assert this.posString().equals(pos);
    }

    public BitBoard(long pos_b, long pos_w, boolean sideToMove) {

        // Load data from the position string into the edu.caltech.cs2.board.
        board_w_0 = pos_w;
        board_b_0 = pos_b;

//        updateBoardsFromNormalized();

        // Set the side to move.
        this.sideToMove = sideToMove;

    }

    public BitBoard(long pos_b, long pos_w, long b_45r, long w_45r, long b_45l, long w_45l, long b_90r, long w_90r, boolean sideToMove) {
//        if (!pos.matches(MOVE_STRING_REGEX)) {
//            // Invalid position string (doesn't match regex.)
//            // TODO Add some sort of exception here.
//        }

        // Load data from the position string into the edu.caltech.cs2.board.
        board_w_0 = pos_w;
        board_b_0 = pos_b;
        board_w_45r = w_45r;
        board_b_45r = b_45r;
        board_w_45l = w_45l;
        board_b_45l = b_45l;
        board_w_90r = w_90r;
        board_b_90r = b_90r;

        // Set the side to move.
        this.sideToMove = sideToMove;

    }

//    private void updateBoardsFromNormalized() {
//        board_w_45l = rotate_45l(board_w_0);
//        board_b_45l = rotate_45l(board_b_0);
//
//        board_w_45r = rotate_45r(board_w_0);
//        board_b_45r = rotate_45r(board_b_0);
//
//        board_w_90r = rotate90r(board_w_0);
//        board_b_90r = rotate90r(board_b_0);
//    }
//
//    private long set(Long board, int n) {
//        return board | n_vals[n];
//    }
//
//    private long unset(Long board, int n) {
//        return board & ~n_vals[n];
//    }
//
//    private long flipVertical(long x) {
//        x = ((x >>>  8) & 0x00FF00FF00FF00FFL) | ((x & 0x00FF00FF00FF00FFL) <<  8);
//        x = ((x >>> 16) & 0x0000FFFF0000FFFFL) | ((x & 0x0000FFFF0000FFFFL) << 16);
//        x = ( x >>> 32)       | ( x       << 32);
//        return x;
//    }
//
//    private long rotate90r (long board) {
//        return flipVertical (flipDiagA1H8 (board));
//    }
//
//    private long flipDiagA1H8(long board) {
//        long t;
//        t  = 0x0f0f0f0f00000000L & (board ^ (board << 28));
//        board ^= t ^ (t >>> 28) ;
//        t  = 0x3333000033330000L & (board ^ (board << 14));
//        board ^= t ^ (t >>> 14) ;
//        t  = 0x5500550055005500L & (board ^ (board <<  7));
//        board ^= t ^ (t >>>  7) ;
//        return board;
//    }
//
//    private long rotate_45r (long board) {
//        board ^= 0xAAAAAAAAAAAAAAAAL & (board ^ rotateRight(board,  8));
//        board ^= 0xCCCCCCCCCCCCCCCCL & (board ^ rotateRight(board, 16));
//        board ^= 0xF0F0F0F0F0F0F0F0L & (board ^ rotateRight(board, 32));
//        return board;
//    }
//
//    private long rotate_45l (long board) {
//        board ^= 0x5555555555555555L & (board ^ rotateRight(board,  8));
//        board ^= 0x3333333333333333L & (board ^ rotateRight(board, 16));
//        board ^= 0x0F0F0F0F0F0F0F0FL & (board ^ rotateRight(board, 32));
//        return board;
//    }
//
//    private int transform_into_45r (int square) {
//        return (square + 8*(square&7)) & 63;
//    }
//
//    private int transform_into_45l (int square) {
//        return (square + 8*((square&7)^7)) & 63;
//    }
//
//    private int transform_into_90r (int square) {
//        return (((square >>> 3) | (square << 3)) & 63) ^ 56;
//    }
//
//    private int normalize_45r (int square) {
//        return BitBoard_precomp.t_45rInverse[square];
//    }
//
//    private int normalize_45l (int square) {
//        return BitBoard_precomp.t_45lInverse[square];
//    }
//
//    private int normalize_90r (int square) {
//        return (((square >>> 3) | (square << 3)) & 63) ^ 7;
//    }


    public void makeMove(Move move) {

        sideToMove = !sideToMove;

        UndoInfo undo = new UndoInfo(move, board_b_0, board_w_0);
        moves.addBack(undo);

        if (move.isPass()) {
            return;
        }

        --moves_remaining; // only decrement if

        long x, end_disk;
        long placed_disk = n_vals[move.getSquare()];
        long captured_disks = 0;

        long own_board = sideToMove ? board_b_0 | placed_disk : board_w_0 | placed_disk;
        long opp_board = sideToMove ? board_w_0 : board_b_0;

        for (int dir = 0; dir < 8; ++dir) {
            /* Find opponent disk adjacent to the new disk. */
            x = maskedShift(placed_disk, dir) & opp_board;

            /* Add any adjacent opponent disk to that one, and so on. */
            x |= maskedShift(x, dir) & opp_board;
            x |= maskedShift(x, dir) & opp_board;
            x |= maskedShift(x, dir) & opp_board;
            x |= maskedShift(x, dir) & opp_board;
            x |= maskedShift(x, dir) & opp_board;

            /* Determine whether the disks were captured. */
            end_disk = maskedShift(x, dir) & own_board;
            captured_disks |= (end_disk!=0 ? x : 0);
        }

        // XOR boards with captured squares
        this.board_b_0 ^= captured_disks;
        this.board_w_0 ^= captured_disks;

    }

    public void undoMove() {
        UndoInfo undo = moves.removeBack();
        sideToMove = !sideToMove;
        if (undo.move.isPass()) return;

        // reverse side to move for undo:
        this.board_b_0 = undo.prev_board_b;
        this.board_w_0 = undo.prev_board_w;

        // update boards
//        updateBoardsFromNormalized();
    }

    private long maskedShift(long board, int dir) {

        if (dir < 4) { // right shift
            return (board >>> RSHIFTS[dir]) & dirs[dir];
        } else { // left shift
            return (board << LSHIFTS[dir]) & dirs[dir];
        }
    }

    public IDeque<Move> getMoves() {

        IDeque<Move> moves = new ArrayDeque<>(24);

        long opp = sideToMove ? board_w_0 : board_b_0;
        long own = sideToMove ? board_b_0 : board_w_0;
        long empty = ~(opp | own);

        long legal_moves = 0;

        for (int dir = 0; dir < 8; dir++) {
            /* Get opponent disks adjacent to my disks in direction dir. */
            long x = maskedShift(own, dir) & opp;

            /* Add opponent disks adjacent to those, and so on. */
            x |= (maskedShift(x, dir)) & opp;
            x |= (maskedShift(x, dir)) & opp;
            x |= (maskedShift(x, dir)) & opp;
            x |= (maskedShift(x, dir)) & opp;
            x |= (maskedShift(x, dir)) & opp;

            /* Empty cells adjacent to those are valid moves. */
            legal_moves |= (maskedShift(x, dir)) & empty;
        }

        for (int i = 0; i < 64; ++i) {
            if (legal_moves >>> n_vals[i] == 1) moves.addBack(new Move(i));
        }

        if (moves.size() == 0) {
            moves.addBack(new Move(Move.PASS));
        }

        return moves;

    }

    private int popcount64(long x) {
        int count;
        for (count = 0; x != 0; ++count) {
            x &= x - 1;
        }
        return count;
    }

    private int[] diskCount() {
        return new int[]{Long.bitCount(board_b_0), Long.bitCount(board_w_0)};
        // use the Java builtin, which is more efficient on many filled bits (17 operations)
//        if (moves_remaining < 44) return new int[]{Long.bitCount(board_b_0), Long.bitCount(board_w_0)};
//        // otherwise use the more efficient popcount64 algorithm if we expect few filled bits, i.e. still early game
//        return new int[]{popcount64(board_b_0), popcount64(board_w_0)};

    }

    public boolean isGameOver() {
        if (moves_remaining <= 0) return true;
        int[] diskCounts = diskCount();
        if (diskCounts[0] == 0 || diskCounts[1] == 0 || diskCounts[0]+diskCounts[1] == 64) return true;
//        IDeque<Move> moves = getMoves();
//        if (moves.size() == 1 && moves.peek().isPass()) return true;
        return false;
    }

    public boolean isBlackMove() {
        return sideToMove;
    }

    public int getScore() {
        int[] diskCounts = diskCount(); // positive if black is ahead
        return diskCounts[0] - diskCounts[1];
    }

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
                CORNER_WEIGHT * ((board_b_0 << n_vals[0]) + (board_b_0 << n_vals[7]) +
                     (board_b_0 << n_vals[56]) + (board_b_0 << n_vals[63])) +
                CORNER_ADJACENT_WEIGHT * ((board_b_0 << n_vals[0]) + (board_b_0 << n_vals[7]) +
                        (board_b_0 << n_vals[56]) + (board_b_0 << n_vals[63]))
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
        int count = 0;
        for (int i = 0; i < 64; ++i) {
            if ((board_w_0 & n_vals[i]) == 1) ret += WHITE_CHAR;
            else if ((board_b_0 & n_vals[i]) == 1) ret += BLACK_CHAR;
            else ret += EMPTY_CHAR;
            ++count;
            if (count % 8 == 0) {
                ret += " │\n"+(int)((count/8)+1)+"  │ ";
            }
        }
        ret += "  └─────────────────┘\n";
        ret += "    A B C D E F G H\n";
        return ret;
    }

    public String posString() {
        char[] pos = new char[66];
        for (int i = 0; i < 64; ++i) {
            if ((board_w_0 & n_vals[i]) == 1) pos[i] = WHITE_CHAR;
            else if ((board_b_0 & n_vals[i]) == 1) pos[i] = BLACK_CHAR;
            else pos[i] = EMPTY_CHAR;
        }

        pos[64] = ' ';
        pos[65] = (sideToMove ? BLACK_CHAR : WHITE_CHAR);
        return new String(pos);
    }

    private class UndoInfo {
        public long prev_board_b;
        public long prev_board_w;
        public Move move;

        public UndoInfo(Move move, long black, long white) {
            this.move = move;
            this.prev_board_w = white;
            this.prev_board_b = black;
        }
    }

    public BitBoard copy() {
        return new BitBoard(board_b_0, board_w_0,
                            board_b_45r, board_w_45r,
                            board_b_45l, board_w_45l,
                            board_b_90r, board_w_90r,
                            sideToMove);
    }
}
