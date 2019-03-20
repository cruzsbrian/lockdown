package project08.board;

import java.util.HashMap;

public class BitBoard_precomp {

    // (1 << n) for 0<n<64
    protected static final long[] n_vals = {1L, 2L, 4L, 8L, 16L, 32L, 64L, 128L, 256L, 512L, 1024L, 2048L, 4096L, 8192L, 16384L, 32768L,
            65536L, 131072L, 262144L, 524288L, 1048576L, 2097152L, 4194304L, 8388608L, 16777216L, 33554432L, 67108864L,
            134217728L, 268435456L, 536870912L, 1073741824L, 2147483648L, 4294967296L, 8589934592L, 17179869184L, 34359738368L,
            68719476736L, 137438953472L, 274877906944L, 549755813888L, 1099511627776L, 2199023255552L, 4398046511104L,
            8796093022208L, 17592186044416L, 35184372088832L, 70368744177664L, 140737488355328L, 281474976710656L, 562949953421312L,
            1125899906842624L, 2251799813685248L, 4503599627370496L, 9007199254740992L, 18014398509481984L, 36028797018963968L,
            72057594037927936L, 144115188075855872L, 288230376151711744L, 576460752303423488L, 1152921504606846976L,
            2305843009213693952L, 4611686018427387904L, 0xFFFFFFFFFFFFFFFFL};

    // directional magic numbers for move calc
    private static long BB_DOWN = 0x00FFFFFFFFFFFFFFL;
    private static long BB_RIGHT = 0x7F7F7F7F7F7F7F7FL;
    private static long BB_DOWN_RIGHT = 0x007F7F7F7F7F7F7FL;
    private static long BB_DOWN_LEFT = 0x00FEFEFEFEFEFEFEL;
    private static long BB_UP = 0xFFFFFFFFFFFFFF00L;
    private static long BB_LEFT = 0xFEFEFEFEFEFEFEFEL;
    private static long BB_UP_RIGHT = 0x7F7F7F7F7F7F7F00L;
    private static long BB_UP_LEFT = 0xFEFEFEFEFEFEFE00L;
    static final long[] dirs = {BB_DOWN, BB_RIGHT, BB_DOWN_RIGHT, BB_DOWN_LEFT, BB_UP, BB_LEFT, BB_UP_LEFT, BB_UP_RIGHT};

    static final int[] RSHIFTS = {
            8, //  Down
            1, //  Right
            9, //  Down-right
            7, //  Down-left
            0, //  Up
            0, //  Left
            0, //  Up-left
            0  //  Up-right
    };
    static final int[] LSHIFTS = {
            0, //  Down
            0, //  Right
            0, //  Down-right
            0, //  Down-left
            8, //  Up
            1, //  Left
            9, //  Up-left
            7  //  Up-right
    };

    // starting position of each diagonal when translated
//    public static int[] diagonal_lengths = {1,2,3,4,5,6,7,8,7,6,5,4,3,2,1};
    protected static int[] diagonal_start = {0, 1, 8, 10, 16, 19, 24, 28, 32, 37, 40, 46, 48, 55, 56};

    // translation table to return to normal indices from a diagonalized bitboard
    protected static byte[] t_45rInverse = {
            0, 9, 18, 27, 36, 45, 54, 63, 8, 17, 26, 35, 44, 53, 62, 7, 16, 25, 34, 43, 52,
            61, 6, 15, 24, 33, 42, 51, 60, 5, 14, 23, 32, 41, 50, 59, 4, 13, 22, 31, 40, 49,
            58, 3, 12, 21, 30, 39, 48, 57, 2, 11, 20, 29, 38, 47, 56, 1, 10, 19, 28, 37, 46, 55};
    protected static byte[] t_45lInverse = {
            56, 49, 42, 35, 28, 21, 14, 7, 0, 57, 50, 43, 36, 29, 22, 15, 8, 1, 58, 51, 44,
            37, 30, 23, 16, 9, 2, 59, 52, 45, 38, 31, 24, 17, 10, 3, 60, 53, 46, 39, 32, 25,
            18, 11, 4, 61, 54, 47, 40, 33, 26, 19, 12, 5, 62, 55, 48, 41, 34, 27, 20, 13, 6, 63};


}
