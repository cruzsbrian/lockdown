import numpy as np

def board_from_bits(b_bits, w_bits):
    b = []
    w = []
    for ii in range(64):
        b.append(b_bits & 1)
        w.append(w_bits & 1)
        b_bits = b_bits >> 1
        w_bits = w_bits >> 1

    return (b, w)

def print_board(board):
    for ii in range(64):
        if board[0][ii] == 1:
            print('B', end=' ')
        elif board[1][ii] == 1:
            print('W', end=' ')
        else:
            print('_', end=' ')
        if ii % 8 == 7:
            print()

patterns = {
    'diag4': [3, 10, 17, 24],
    'diag5': [4, 11, 18, 25, 32],
    'diag6': [5, 12, 19, 26, 33, 40],
    'diag7': [6, 13, 20, 27, 34, 41, 48],
    'diag8': [7, 14, 21, 28, 35, 42, 49, 56],
    'edge1': [0, 1, 2, 3, 4, 5, 6, 7],
    'edge2': [8, 9, 10, 11, 12, 13, 14, 15],
    'edge3': [16, 17, 18, 19, 20, 21, 22, 23],
    'edge4': [24, 25, 26, 27, 28, 29, 30, 31],
    '2x4': [0, 1, 2, 3, 8, 9, 10, 11],
    '3x3': [0, 1, 2, 8, 9, 10, 16, 17, 18]
}

n_pattern_instances = sum([3**len(x) for x in patterns.values()])

b = board_from_bits(0x0000000810000000, 0x0000001008000000)

print_board(b)
