import numpy as np

def board_from_bits(b_bits, w_bits):
    b = np.empty((8, 8))
    w = np.empty((8, 8))
    for ii in range(8):
        for jj in range(8):
            b[ii][jj] = b_bits & 1
            w[ii][jj] = w_bits & 1
            b_bits = b_bits >> 1
            w_bits = w_bits >> 1

    return (b, w)

def print_board(board):
    for ii in range(8):
        for jj in range(8):
            if board[0][ii][jj] == 1:
                print('B', end=' ')
            elif board[1][ii][jj] == 1:
                print('W', end=' ')
            else:
                print('_', end=' ')
        print()

def fliplr(board):
    return (np.fliplr(board[0]), np.fliplr(board[1]))

def flipud(board):
    return (np.flipud(board[0]), np.flipud(board[1]))

def rot90(board, k):
    return (np.rot90(board[0], k), np.rot90(board[1], k))

def transpose(board):
    return (np.transpose(board[0]), np.transpose(board[1]))

def flip(board):
    return (np.flip(board[0]), np.flip(board[1]))

def extract(board, pattern):
    b = 0
    w = 0
    for index in pattern:
        b = b << 1
        w = w << 1
        if board[0][index] != 0: b += 1
        if board[1][index] != 0: w += 1
    return (b, w)

def extract_transformations(board, pattern):
    result = []

    result.append(extract(board, pattern))
    result.append(extract(fliplr(board), pattern))
    result.append(extract(flipud(board), pattern))
    result.append(extract(rot90(board, 1), pattern))
    result.append(extract(rot90(board, 2), pattern))
    result.append(extract(rot90(board, 3), pattern))
    result.append(extract(transpose(board), pattern))
    result.append(extract(flip(transpose(board)), pattern))

    return result

def extract_all(board, p_list):
    global n_instances

    result = []

    for ii in range(len(p_list)):
        start = sum(n_instances[:ii])
        for x in extract_transformations(board, p_list[ii]):
            result.append(x)

    return result

patterns = {
    'diag4': [(0, 3), (1, 2), (2, 1), (3, 0)],
    'diag5': [(0, 4), (1, 3), (2, 2), (3, 1), (4, 0)],
    'diag6': [(0, 5), (1, 4), (2, 3), (3, 2), (4, 1), (5, 0)],
    'diag7': [(0, 6), (1, 5), (2, 4), (3, 3), (4, 2), (5, 1), (6, 0)],
    'diag8': [(0, 7), (1, 6), (2, 5), (3, 4), (4, 3), (5, 2), (6, 1), (7, 0)],
    'edge1': [(0, 0), (0, 1), (0, 2), (0, 3), (0, 4), (0, 5), (0, 6), (0, 7)],
    'edge2': [(1, 0), (1, 1), (1, 2), (1, 3), (1, 4), (1, 5), (1, 6), (1, 7)],
    'edge3': [(2, 0), (2, 1), (2, 2), (2, 3), (2, 4), (2, 5), (2, 6), (2, 7)],
    'edge4': [(3, 0), (3, 1), (3, 2), (3, 3), (3, 4), (3, 5), (3, 6), (3, 7)],
    '2x4': [(0, 0), (0, 1), (0, 2), (0, 3), (1, 0), (1, 1), (1, 2), (1, 3)],
    '3x3': [(0, 0), (0, 1), (0, 2), (1, 0), (1, 1), (1, 2), (2, 0), (2, 1), (2, 2)]
}

n_instances = [2**(2 * len(x)) for x in patterns.values()]
print(sum(n_instances))

n_trials = 12000

A = np.zeros((n_trials, sum(n_instances)), dtype=np.int8)
b = np.empty(n_trials)

with open('data.txt', 'r') as f:
    for ii in range(n_trials):
        line = f.readline().split(' ')

        board = board_from_bits(int(line[0], 16), int(line[1], 16))

        matches = extract_all(board, [p for p in patterns.values()])

        b[ii] = int(line[4])
