import numpy as np

n_trials = 200000
n_params = 7

a = np.zeros((n_trials, n_params), dtype=np.int8)
b = np.empty(n_trials)

with open('data/15-18.txt', 'r') as f:
    for ii in range(n_trials):
        line = f.readline().split(' ')

        b[ii] = int(line[4])

        a[ii] = [int(x) for x in f.readline().split(' ')];

# print(a)
# print(b)

ainv = np.linalg.pinv(a)
result = np.array(np.matmul(ainv, b))

mean = np.mean(b)
sstot = np.sum((b - mean)**2)

f = np.array(np.matmul(a, result))
ssres = np.sum((f - b)**2)

print(result)

print(1 - (ssres / sstot))
