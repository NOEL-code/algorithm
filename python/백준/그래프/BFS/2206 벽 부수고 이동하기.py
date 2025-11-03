import sys
from collections import deque
input = sys.stdin.readline

N, M = map(int,input().split())

grid = [list(map(int, input().strip())) for _ in range(N)]
directions = [(1, 0), (0, 1), (-1, 0), (0, -1)]

visited = [[[0]*2] for _ in range(M) for __ in range(N)]
def bfs():

    queue = deque([0,0,0])
    visited[0][0][0] = 1

    while queue:
        x, y, broken = queue.popleft()

        if x == N-1 and y == M-1:
            return visited[x][y][broken]
        
        for dx, dy in directions:
            nx, ny = dx + x, dy + y

        if 0 <= nx < N and 0 <= ny < M:
            if grid[nx][ny] == 0 and visited[nx][ny][broken] == 0:
                visited[nx][ny][broken] = visited[x][y][broken] + 1
                queue.append((x, y, broken))
            elif grid[nx][ny] == 1 and visited[nx][ny][1] == 0 and broken == 0:
                visited[nx][ny][1] = visited[x][y][broken] + 1
                queue.append((x, y, 1))

ans = bfs()
print(ans)

