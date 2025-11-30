import sys
from collections import deque

input = sys.stdin.readline

N, M = map(int, input().split())

treasure_map = [list(input().strip()) for _ in range(N)]  # N줄, 각 줄은 문자 리스트

directions = [(0,1), (1,0), (-1,0), (0,-1)]

def BFS(y, x):
    queue = deque([(y,x)])

    visited = [[-1] * M for _ in range(N)]
    visited[y][x] = 0
    max_dist = 0
    while queue:
        y, x = queue.popleft()

        for dy, dx in directions:
            ny, nx = y + dy, x + dx

            if 0 <= ny < N and 0 <= nx < M:
                if  visited[ny][nx] == -1 and treasure_map[ny][nx] == 'L':
                    visited[ny][nx] = visited[y][x] + 1
                    max_dist = max(max_dist, visited[ny][nx])
                    queue.append((ny,nx))
    return max_dist

ans = 0

for i in range(M):
    for j in range(N):
        if treasure_map[j][i] == 'L':
            dist = BFS(j,i)

            ans = max(dist, ans)

print(ans)