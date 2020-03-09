import numpy as np
import pandas as pd
from sklearn.linear_model import LogisticRegression
import random
import sys

def score_to_win(score):
    return np.sign(score)

n_trials = 200000
n_params = 8

a = np.zeros((n_trials, n_params), dtype=np.int8)
b = np.empty(n_trials)

with open(sys.argv[1], 'r') as f:
    for ii in range(n_trials):
        line = f.readline().split(' ')

        a[ii] = [int(x) for x in f.readline().split(' ')]
        b[ii] = int(line[4])

# print(a)
# print(b)

# model = LogisticRegression(solver='liblinear')
# model.fit(a, b)
# print(model.intercept_)
# print(np.rint(model.coef_ * 500))
# print("Model score:", model.score(a, b))

ainv = np.linalg.pinv(a)
result = np.array(np.matmul(ainv, b))

mean = np.mean(b)
sstot = np.sum((b - mean)**2)

f = np.array(np.matmul(a, result))
ssres = np.sum((f - b)**2)

print("Weights:", np.rint(result))

print("R^2:", 1 - (ssres / sstot))
