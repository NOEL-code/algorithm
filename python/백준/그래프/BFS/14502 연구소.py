import sys
from collections import deque
from itertools import combinations
input = sys.stdin.readline

N, M = map(int, input().split())

grid = [list(map(int, input().split())) for _ in range(N)]
directions = [(0, 1), (-1, 0), (1, 0), (0, -1)]
    

def BFS(temp):
    queue = deque()

    for i in range(N):
        for j in range(M):
            if temp[i][j] == 2:
                queue.append((i, j))

    while queue:
        x, y = queue.popleft()

        for dx, dy in directions:
            nx, ny = dx + x, dy + y
            if 0 <= nx < N and 0 <= ny < M:
                if temp[nx][ny] == 0:
                    temp[nx][ny] = 2
                    queue.append((nx,ny))
    safe = sum(row.count(0) for row in temp)
    return safe

empty = [(i, j) for i in range(N) for j in range(M) if grid[i][j] == 0]

ans = 0
for walls in combinations(empty, 3):
    temp = [row[:] for row in grid]
    for x, y in walls:
        temp[x][y] = 1
    ans = max(ans, BFS(temp))

print(ans)

