import sys

input = sys.stdin.readline

N, L = map(int, input().split())
board = []
for _ in range(N):
    board.append(list(map(int, input().split())))

def check(line):
    used = [False] * N
    for i in range(N - 1):
        diff = line[i+1] - line[i]
        if diff == 0:
            continue
        elif diff == 1:
            for j in range(i, i - L, -1):
                if j < 0 or line[j] != line[i] or used[j]:
                    return False
                used[j] = True
        elif diff == -1:
            for j in range(i + 1, i + 1 + L):
                if j >= N or line[j] != line[i+1] or used[j]:
                    return False
                used[j] = True
        else:
            return False
    return True

result = 0

for i in range(N):
    if check(board[i]):
        result += 1
    col = [board[r][i] for r in range(N)]
    if check(col):
        result += 1

print(result)