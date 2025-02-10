import sys
from collections import deque

input = sys.stdin.readline

N, M = map(int, input().split())
graph = [list(map(str, input().rstrip())) for _ in range(N)]
visited = [[False] * M for _ in range(N)]

def bfs(x, y):
    directions = [(1, 0), (0, 1), (-1, 0), (0, -1)]
    queue = deque([(x, y)])
    visited[x][y] = True
    count = 0
    
    while queue:
        x, y = queue.popleft()
        
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            
            if 0 <= nx < N and 0 <= ny < M:
                if not visited[nx][ny] and graph[nx][ny] != 'X':
                    visited[nx][ny] = True
                    queue.append((nx, ny))
                    
                    if graph[nx][ny] == 'P':
                        count += 1

    return count

for i in range(N):
    for j in range(M):
        if graph[i][j] == 'I':
            count = bfs(i, j)
            if count > 0:
                print(count)
            else:
                print('TT')
            

        
                    