import sys
from collections import deque

input = sys.stdin.readline
N = int(input())

graph = [list(input().strip()) for _ in range(N)]

def bfs_color_weakness(x, y, color):
    directions = [(1, 0), (0, 1), (-1, 0), (0, -1)]
    queue = deque([(x, y)])
    visited[x][y] = True
    
    while queue:
        x, y = queue.popleft()
        
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            
            if 0 <= nx < N and 0 <= ny < N:
                if not visited[nx][ny]:
                    if color in ('G', 'R'):
                        if graph[nx][ny] in ('G', 'R'):
                            visited[nx][ny] = True
                            queue.append((nx, ny))

                    elif graph[nx][ny] == color:
                        visited[nx][ny] = True
                        queue.append((nx, ny))

def bfs(x, y, color):
    directions = [(1, 0), (0, 1), (-1, 0), (0, -1)]
    queue = deque([(x, y)])
    visited[x][y] = True
    
    while queue:
        x, y = queue.popleft()
        
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            
            if 0 <= nx < N and 0 <= ny < N:
                if not visited[nx][ny] and graph[nx][ny] == color:
                    visited[nx][ny] = True
                    queue.append((nx, ny))
    
    
count = 0
visited = [[False] * N for _ in range(N)]
for i in range(N):
    for j in range(N):
        if not visited[i][j]:
            bfs(i, j, graph[i][j])
            count += 1

color_weakness_count = 0   
visited = [[False] * N for _ in range(N)]
for i in range(N):
    for j in range(N):
        if not visited[i][j]:
            bfs_color_weakness(i, j, graph[i][j])
            color_weakness_count += 1
            
            
            
print(count, color_weakness_count)