import sys
from collections import deque

input = sys.stdin.readline

T = int(input())

def bfs(x, y, graph, visited, M, N):
    directions = [(1, 0), (0, 1), (-1, 0), (0, -1)]
    queue = deque([x, y])
    visited[y][x] = True
    
    while queue:
        x, y = queue.popleft()
        
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            if 0 <= nx < M and 0 <= ny < N:
                if not visited[ny][nx] and graph[ny][nx] == -1:
                    visited[ny][nx] = True
                    queue.append((nx, ny))

for _ in range(T):
    M, N, K = map(int, input().split())
    
    graph = [[0] * M for _ in range(N)]
    visited = [[False] * M for _ in range(N)]
    
    for _ in range(K):
        x, y = map(int, input().split())
        graph[y][x] = 1
        
    count = 0
    for i in range(N):
        for j in range(M):
            if graph[i][j] == 1 and not visited[i][j]:
                bfs(j, i, graph, visited, M, N)
                count += 1
                
    print(count)