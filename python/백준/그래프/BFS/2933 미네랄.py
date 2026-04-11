import sys
from collections import deque

input = sys.stdin.readline

R, C = map(int, input().split())

grid = [list(input().rstrip()) for _ in range(R)]

N = int(input())
H = list(map(int, input().split()))

directions = [(0, 1), (1, 0), (-1, 0), (0, -1)]

def thorw(turn, height):
    row = R - height
    if turn % 2 == 0:
        for c in range(C):
            if grid[row][c] == 'x':
                grid[row][c] = '.'
                return
            
    else:
        for c in range(C - 1, -1, -1):
            if grid[row][c] == 'x':
                grid[row][c] = '.'
                return
    
def find_floating():
    visited = [[False] * C for _ in range(R)]
    q = deque()
    for c in range(C):
        if grid[R-1][c] == 'x':
            visited[R-1][c] = True
            q.append((R-1, c))
    
    while q:
        x, y = q.popleft()
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            if 0 <= nx < R and 0 <= ny < C and not visited[nx][ny] and grid[nx][ny] == "x":
                visited[nx][ny] = True
                q.append((nx,ny))
    
    floating = []
    for r in range(R):
        for c in range(C):
            if grid[r][c] == 'x' and not visited[r][c]:
                floating.append((r,c))
    return floating

def drop(floating):
    floating_set = set(floating)
    move = R
    for r, c in floating:
        dist = 0
        nr = r + 1
        while nr < R and (nr, c) not in floating_set and grid[nr][c] != 'x':
            dist += 1
            nr += 1
        move = min(move, dist)
    if move == 0:
        return
    for r, c in floating:
        grid[r][c] = '.'
    for r, c in floating:
        grid[r + move][c] = 'x'

for turn, h in enumerate(H):
    thorw(turn, h)
    floating = find_floating()
    if floating:
        drop(floating)

for row in grid:
    print(''.join(row))