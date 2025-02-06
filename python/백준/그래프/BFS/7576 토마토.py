import sys
from collections import deque

input = sys.stdin.readline

# 입력 받기
m, n = map(int, input().split())  # n: 세로(행), m: 가로(열)

graph = []
queue = deque()

for i in range(n):
    row = list(map(int, input().split()))  # 한 줄 입력받기
    graph.append(row)
    for j in range(m):
        if row[j] == 1:  # 익은 토마토(1)를 BFS의 시작점으로 큐에 저장
            queue.append((j, i))  # x, y 순서로 저장 (가로, 세로)

# 방향 설정 (상, 하, 좌, 우)
directions = [(1, 0), (0, 1), (-1, 0), (0, -1)]

# BFS 실행
def bfs():
    while queue:
        x, y = queue.popleft()
        
        for dx, dy in directions:
            nx, ny = x + dx, y + dy
            
            if 0 <= nx < m and 0 <= ny < n:  # 범위 체크
                if graph[ny][nx] == 0:  # 안 익은 토마토만 익힐 수 있음
                    graph[ny][nx] = graph[y][x] + 1  # 날짜 갱신
                    queue.append((nx, ny))  # 새로운 익은 토마토 추가

bfs()  # BFS 실행

# 최대 날짜 계산 (최소 일수)
max_days = 0
for row in graph:
    for value in row:
        if value == 0:  # 안 익은 토마토가 남아있다면 -1 출력
            print(-1)
            exit(0)
        max_days = max(max_days, value)

# 최소 날짜 출력 (1부터 시작했으므로 -1 해줌)
print(max_days - 1)