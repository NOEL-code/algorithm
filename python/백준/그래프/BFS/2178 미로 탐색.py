import sys
from collections import deque

input = sys.stdin.readline

N, M = map(int, input().split())

maze = []

for _ in range(N):
    maze.append(list(map(int, input().strip())))

directions = [(1, 0), (0, 1), (-1, 0), (0, -1)]

def bfs():
    queue = deque([(0, 0)])
    
    while queue:
        x, y = queue.popleft()
        
        for dx, dy in directions:
            nx, ny = dx + x, dy + y
            
            if 0 <= nx < M and 0 <= ny < N:
                if maze[ny][nx] == 1:
                    maze[ny][nx] = maze[y][x] + 1
                    queue.append((nx, ny))

bfs()

print(maze[N -1][M - 1])                