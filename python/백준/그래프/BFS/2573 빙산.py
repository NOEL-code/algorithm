import sys
from collections import deque

input = sys.stdin.readline

N, M = map(int, input().split())
grid = [list(map(int, input().split())) for _ in range(N)]

directions = [(-1,0), (0,-1), (1,0), (0,1)]

def bfs(sx, sy, visited):
    queue = deque([(sx, sy)])
    visited[sx][sy] = True

    while queue:
        x, y = queue.popleft()

        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            if 0 <= nx < N and 0 <= ny < M:
                if not visited[nx][ny] and grid[nx][ny] > 0:
                    visited[nx][ny] = True
                    queue.append((nx, ny))


def melt():
    # 각 칸이 얼마나 녹는지 먼저 계산
    minus = [[0] * M for _ in range(N)]

    for i in range(N):
        for j in range(M):
            if grid[i][j] > 0:
                sea_cnt = 0
                for dx, dy in directions:
                    ni, nj = i + dx, j + dy
                    if 0 <= ni < N and 0 <= nj < M:
                        if grid[ni][nj] == 0:
                            sea_cnt += 1
                minus[i][j] = sea_cnt

    # 실제로 녹이기
    for i in range(N):
        for j in range(M):
            if grid[i][j] > 0:
                grid[i][j] -= minus[i][j]
                if grid[i][j] < 0:
                    grid[i][j] = 0


year = 0

while True:
    # 1. 현재 빙산 덩어리 개수 세기
    visited = [[False] * M for _ in range(N)]
    count = 0

    for i in range(N):
        for j in range(M):
            if grid[i][j] > 0 and not visited[i][j]:
                bfs(i, j, visited)
                count += 1

    if count >= 2:
        print(year)
        break

    if count == 0:
        print(0)
        break

    # 2. 빙산 녹이기
    melt()
    year += 1