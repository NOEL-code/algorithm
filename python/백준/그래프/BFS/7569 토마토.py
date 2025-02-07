import sys
from collections import deque

input = sys.stdin.readline

# 입력 받기
m, n, h = map(int, input().split())  # n: 세로(행), m: 가로(열)

graph = [[[] for _ in range(n)] for _ in range(h)]
queue = deque()

for z in range(h):
    for y in range(n):
        row = list(map(int, input().split()))  # 한 줄 입력받기
        graph[z][y] = row
        for x in range(m):
            if row[x] == 1:  # 익은 토마토(1)를 BFS의 시작점으로 큐에 저장
                queue.append((x, y, z))  # x, y 순서로 저장 (가로, 세로)

# 방향 설정 (상, 하, 좌, 우, 앞, 뒤)
directions = [(1, 0, 0), (0, 1, 0), (-1, 0, 0), (0, -1, 0), (0, 0, 1), (0, 0, -1)]

# BFS 실행
def bfs():
    while queue:
        x, y, z = queue.popleft()
        
        for dx, dy, dz in directions:
            nx, ny, nz = x + dx, y + dy, z + dz
            
            if 0 <= nx < m and 0 <= ny < n and 0 <= nz < h:  # 범위 체크
                if graph[nz][ny][nx] == 0:  # 안 익은 토마토만 익힐 수 있음
                    graph[nz][ny][nx] = graph[z][y][x] + 1  # 날짜 갱신
                    queue.append((nx, ny, nz))  # 새로운 익은 토마토 추가

bfs()  # BFS 실행

# 최대 날짜 계산 (최소 일수)
max_days = 0
for z in range(h):
    for y in range(n):
        for x in range(m):
            if graph[z][y][x] == 0:  # 안 익은 토마토가 남아있다면 -1 출력
                print(-1)
                exit(0)
            max_days = max(max_days, graph[z][y][x])

# 최소 날짜 출력 (1부터 시작했으므로 -1 해줌)
print(max_days - 1)