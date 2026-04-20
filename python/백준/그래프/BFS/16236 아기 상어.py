import sys
from collections import deque

input = sys.stdin.readline

N = int(input())
grid = [list(map(int, input().split())) for _ in range(N)]

start_i, start_j = 0, 0
for i in range(N):
    for j in range(N):
        if grid[i][j] == 9:
            start_i, start_j = i, j

directions = [(0, 1), (1, 0), (-1, 0), (0, -1)]

def find_candidate(start_i, start_j, baby_weight):
    visited = [[-1] * N for _ in range(N)]
    queue = deque([(start_i, start_j)])
    visited[start_i][start_j] = 0
    candidate_list = []

    while queue:
        x, y = queue.popleft()

        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            if 0 <= nx < N and 0 <= ny < N and visited[nx][ny] == -1 and grid[nx][ny] <= baby_weight:
                visited[nx][ny] = visited[x][y] + 1
                if 0 < grid[nx][ny] < baby_weight:
                    candidate_list.append((visited[nx][ny], nx, ny))
                    # 먹을 수 있는 칸은 큐에 추가 안 함
                else:
                    queue.append((nx, ny))

    if candidate_list:
        candidate_list.sort()
        dist, ri, rj = candidate_list[0]
        return [ri, rj, dist]
    return None

time = 0
baby_weight = 2
eat_count = 0

grid[start_i][start_j] = 0

while True:
    result = find_candidate(start_i, start_j, baby_weight)
    if not result:
        print(time)
        break

    next_i, next_j, spent_time = result
    time += spent_time
    eat_count += 1

    if eat_count == baby_weight:
        baby_weight += 1
        eat_count = 0

    grid[next_i][next_j] = 0
    start_i, start_j = next_i, next_j