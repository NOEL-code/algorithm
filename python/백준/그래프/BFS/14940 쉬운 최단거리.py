import sys
from collections import deque

input = sys.stdin.readline

# 입력 받기
n, m = map(int, input().split())  # n: 세로(행), m: 가로(열)
graph = []
target_x, target_y = None, None  # 목표 지점(2)의 위치

for i in range(n):
    row = list(map(int, input().split()))
    graph.append(row)
    for j in range(m):
        if row[j] == 2:  # 목표 지점 찾기
            target_x, target_y = j, i

# 결과 배열 및 방문 배열 초기화
result = [[-1] * m for _ in range(n)]  # 모든 갈 수 있는 땅을 -1로 초기화
visited = [[False] * m for _ in range(n)]  # 방문 여부 체크

# BFS 함수
def bfs(start_x, start_y):
    direction = [(1, 0), (0, 1), (-1, 0), (0, -1)]  # 이동 방향 (상, 하, 좌, 우)
    queue = deque([(start_x, start_y)])
    result[start_y][start_x] = 0  # 목표 지점 거리 0
    visited[start_y][start_x] = True  # 방문 처리

    while queue:
        x, y = queue.popleft()

        for dx, dy in direction:
            nx, ny = x + dx, y + dy

            # 범위 내에 있고 방문하지 않은 경우
            if 0 <= nx < m and 0 <= ny < n:
                if not visited[ny][nx] and graph[ny][nx] == 1:  # 1(갈 수 있는 땅)일 때만 이동
                    visited[ny][nx] = True
                    result[ny][nx] = result[y][x] + 1  # 거리 갱신
                    queue.append((nx, ny))

# 목표 지점(2)에서 BFS 실행
if target_x is not None and target_y is not None:
    bfs(target_x, target_y)

# 갈 수 없는 지역(0)은 0으로 유지
for i in range(n):
    for j in range(m):
        if graph[i][j] == 0:  # 벽이면 결과를 0으로 설정
            result[i][j] = 0

# 결과 출력
for row in result:
    print(*row)