import sys
from collections import deque

input = sys.stdin.readline

N, L, R = map(int, input().split())

directions = [(0, 1), (1, 0), (-1, 0), (0, -1)]

population = [list(map(int, input().split())) for _ in range(N)]


def bfs(x, y, visited):

    queue = deque([(x, y)])
    visited[x][y] = True
    group = [(x, y)]
    total = population[x][y]
    
    while queue:
        x, y = queue.popleft()
        
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            
            if 0 <= nx < N and 0 <= ny < N and not visited[nx][ny]:
                if L <= abs(population[x][y] - population[nx][ny]) <= R:
                    visited[nx][ny] = True
                    group.append((nx, ny))
                    total += population[nx][ny]
                    queue.append((nx, ny))
    
    return group, total

answer = 0
                                
while True:
    remain = False
    
    visited = [[False] * N for _ in range(N)]
    
    for i in range(N):
        for j in range(N):
            if not visited[i][j]:   
                group, total = bfs(i, j, visited)
                if len(group) > 1:     
                    remain = True
                    new_population = total // len(group)
                    for x, y in group:
                        population[x][y] = new_population
    
    
    if not remain:
        break
    answer += 1

print(answer)