import sys
from collections import deque

input = sys.stdin.readline

N = int(input())
graph = [list(map(int, input())) for _ in range(N)]
visited = [[False] * N for _ in range(N)]

directions = [(1, 0), (0, 1), (-1, 0), (0, -1)]

def bfs(start_y, start_x):
    queue = deque([(start_y, start_x)])
    visited[start_y][start_x] = True 
    count = 1
    
    while queue:
        y, x = queue.popleft()

        for dy, dx in directions:
            ny, nx = y + dy, x + dx

            if 0 <= ny < N and 0 <= nx < N and not visited[ny][nx] and graph[ny][nx] == 1:
                visited[ny][nx] = True 
                queue.append((ny, nx))
                count += 1  

    return count

result = []
for i in range(N):
    for j in range(N):
        if graph[i][j] == 1 and not visited[i][j]:
            result.append(bfs(i, j))

result.sort()  
print(len(result))
for r in result:
    print(r)