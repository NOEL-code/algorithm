from collections import deque
import sys

input = sys.stdin.readline

# 6방향 이동: (층, 행, 열)
DZ = [1, -1, 0, 0, 0, 0]   # 위층, 아래층
DY = [0, 0, 1, -1, 0, 0]   # 아래, 위
DX = [0, 0, 0, 0, 1, -1]   # 오른쪽, 왼쪽


def bfs(start_z, start_y, start_x):
    """시작 위치(start_z, start_y, start_x)에서 출발하는 BFS."""
    queue = deque()
    queue.append((start_z, start_y, start_x))
    dist[start_z][start_y][start_x] = 1  # 시작을 1분으로 두고, 도착 시 -1 안 쓰게

    while queue:
        z, y, x = queue.popleft()

        for dir_idx in range(6):
            nz = z + DZ[dir_idx]
            ny = y + DY[dir_idx]
            nx = x + DX[dir_idx]

            # 범위 체크
            if 0 <= nz < L and 0 <= ny < R and 0 <= nx < C:
                cell = building[nz][ny][nx]

                # 출구면 현재 시간(dist[z][y][x])이 곧 최단 시간
                if cell == 'E':
                    print("Escaped in {0} minute(s).".format(dist[z][y][x]))
                    return

                # 빈 칸이고, 아직 방문 안 했으면 이동
                if cell == '.' and dist[nz][ny][nx] == 0:
                    dist[nz][ny][nx] = dist[z][y][x] + 1
                    queue.append((nz, ny, nx))

    # 큐를 다 돌 때까지 E를 못 만났다면 탈출 불가
    print("Trapped!")


while True:
    L, R, C = map(int, input().split())  # 층 수, 행 수, 열 수
    if L == 0 and R == 0 and C == 0:
        break

    # building[z][y][x]
    building = [[[''] * C for _ in range(R)] for _ in range(L)]
    # dist[z][y][x] : (z, y, x)에 도달하는 데 걸린 시간 (0이면 미방문)
    dist = [[[0] * C for _ in range(R)] for _ in range(L)]

    start_z = start_y = start_x = -1

    for z in range(L):
        for y in range(R):
            row = list(input().strip())  # 길이 C짜리 문자열
            for x, ch in enumerate(row):
                building[z][y][x] = ch
                if ch == 'S':
                    start_z, start_y, start_x = z, y, x
        input()  # 층 사이의 빈 줄

    bfs(start_z, start_y, start_x)