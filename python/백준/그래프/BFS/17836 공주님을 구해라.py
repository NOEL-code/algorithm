import sys
from collections import deque

input = sys.stdin.readline

N, M, T = map(int, input().split())
grid = [list(map(int, input().split())) for _ in range(N)]

# 방향: 오른쪽, 왼쪽, 아래, 위
directions = [(0, 1), (0, -1), (1, 0), (-1, 0)]

# visited[x][y][s] : (x,y)에 도달했을 때 최소 시간
# s = 0 : 검 없음, s = 1 : 검 있음
visited = [[[-1] * 2 for _ in range(M)] for _ in range(N)]


def bfs():
    q = deque()
    q.append((0, 0, 0))  # (x, y, sword)
    visited[0][0][0] = 0

    while q:
        x, y, sword = q.popleft()
        time = visited[x][y][sword]

        # 이미 시간 제한 넘으면 더 갈 필요 없음 (가지치기)
        if time > T:
            continue

        for dx, dy in directions:
            nx, ny = x + dx, y + dy

            if 0 <= nx < N and 0 <= ny < M:
                # 이미 검 있는 상태
                if sword == 1:
                    # 검이 있으면 벽(1)도 통과 가능 → 그냥 방문만 체크
                    if visited[nx][ny][1] == -1:
                        visited[nx][ny][1] = time + 1
                        q.append((nx, ny, 1))
                # 아직 검 없는 상태
                else:
                    # 벽(1)은 못 감
                    if grid[nx][ny] != 1:
                        ns = sword
                        # 검(2)을 주운 경우
                        if grid[nx][ny] == 2:
                            ns = 1
                        if visited[nx][ny][ns] == -1:
                            visited[nx][ny][ns] = time + 1
                            q.append((nx, ny, ns))


bfs()

ans = 10**9

# 검 없이 도착한 경우
if visited[N - 1][M - 1][0] != -1:
    ans = min(ans, visited[N - 1][M - 1][0])
# 검 들고 도착한 경우
if visited[N - 1][M - 1][1] != -1:
    ans = min(ans, visited[N - 1][M - 1][1])

if ans <= T:
    print(ans)
else:
    print("Fail")