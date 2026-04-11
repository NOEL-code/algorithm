import sys
from itertools import product

input = sys.stdin.readline

N, M = map(int, input().split())

board = []
cctv = []

for i in range(N):
    row = list(map(int, input().split()))
    board.append(row)
    for j in range(M):
        if 1 <= row[j] <= 5:
            cctv.append((i, j, row[j]))

types = {
    1: [[0], [1], [2], [3]],
    2: [[0, 2], [1, 3]],
    3: [[0,1], [1, 2], [2, 3], [3, 0]],
    4: [[0, 1, 2], [1, 2, 3], [2, 3, 0], [3, 0 ,1]],
    5: [[0, 1, 2, 3]]
}

dx = [-1, 0, 1, 0]
dy = [0, 1, 0 ,-1]

def watch(b, r, c, d):
    while True:
        r, c = r + dx[d], c + dy[d]
        if r < 0 or r >= N or c < 0 or c >= M:
            break
        if b[r][c] == 6:
            break
        if b[r][c] == 0:
            b[r][c] = '#'

choices = [range(len(types[t])) for _, _, t in cctv]

answer = N * M

for combo in product(*choices):
    b = [row[:] for row in board]
    for idx, choice in enumerate(combo):
        r, c, t = cctv[idx]
        for d in types[t][choice]:  
            watch(b, r, c, d)
    count = sum(row.count(0) for row in b)
    answer = min(answer, count)

print(answer)