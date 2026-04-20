import sys
from collections import deque

input = sys.stdin.readline

R, C = map(int, input().split())
grid = [list(input().rstrip()) for _ in range(R)]
N = int(input())
H = list(map(int, input().split()))

directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]

def throw(turn, height):
    row = R - height
    if turn % 2 == 0:
        rng = range(C)
    else:
        rng = range(C - 1, -1, -1)
    for c in rng:
        if grid[row][c] == 'x':
            grid[row][c] = '.'
            return

def bfs(sr, sc, visited):
    q = deque([(sr, sc)])
    visited[sr][sc] = True
    cluster = [(sr, sc)]
    while q:
        r, c = q.popleft()
        for dr, dc in directions:
            nr, nc = r + dr, c + dc
            if 0 <= nr < R and 0 <= nc < C and not visited[nr][nc] and grid[nr][nc] == 'x':
                visited[nr][nc] = True
                q.append((nr, nc))
                cluster.append((nr, nc))
    return cluster

def drop(cluster):
    cluster_set = set(cluster)
    fall = 0
    while True:
        for r, c in cluster:
            nr = r + fall + 1
            if nr >= R:
                return fall
            if grid[nr][c] == 'x' and (nr, c) not in cluster_set:
                return fall
        fall += 1

def process():
    visited = [[False] * C for _ in range(R)]
    # 바닥에 닿은 클러스터 먼저 마킹
    for c in range(C):
        if grid[R-1][c] == 'x' and not visited[R-1][c]:
            bfs(R-1, c, visited)
    # 떠 있는 클러스터 하나 찾기
    for r in range(R):
        for c in range(C):
            if grid[r][c] == 'x' and not visited[r][c]:
                cluster = bfs(r, c, visited)
                fall = drop(cluster)
                if fall > 0:
                    for cr, cc in cluster:
                        grid[cr][cc] = '.'
                    for cr, cc in cluster:
                        grid[cr + fall][cc] = 'x'
                return

for turn, h in enumerate(H):
    throw(turn, h)
    process()

for row in grid:
    print(''.join(row))