import sys
from collections import deque

input = sys.stdin.readline

# (dy, dx) 형태로 쓰는 게 덜 헷갈림
directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]
INF = int(1e9)

T = int(input())

for _ in range(T):
    w, h = map(int, input().split())

    person_dist = [[-1] * w for _ in range(h)]
    fire_dist = [[INF] * w for _ in range(h)]

    building = []
    fire_queue = deque()
    person_queue = deque()

    for y in range(h):
        row = list(input().rstrip())
        building.append(row)
        for x, ch in enumerate(row):
            if ch == '@':
                start_y, start_x = y, x
                person_dist[start_y][start_x] = 0
                person_queue.append((start_y, start_x))
                building[y][x] = '.'  # 시작 위치도 빈 칸처럼 취급
            elif ch == '*':
                fire_dist[y][x] = 0
                fire_queue.append((y, x))

    # 1) 불 BFS
    while fire_queue:
        y, x = fire_queue.popleft()
        for dy, dx in directions:
            ny, nx = y + dy, x + dx
            if 0 <= ny < h and 0 <= nx < w:
                if building[ny][nx] != '#' and fire_dist[ny][nx] == INF:
                    fire_dist[ny][nx] = fire_dist[y][x] + 1
                    fire_queue.append((ny, nx))

    # 2) 상근이 BFS
    escaped_time = -1

    while person_queue:
        y, x = person_queue.popleft()
        curr_time = person_dist[y][x]

        for dy, dx in directions:
            ny, nx = y + dy, x + dx

            # 범위 밖으로 나가면 탈출 성공
            if not (0 <= ny < h and 0 <= nx < w):
                escaped_time = curr_time + 1
                person_queue.clear()  # BFS 끝내기
                break

            # 범위 안인데, 벽이 아니고 아직 방문 안 한 곳
            if building[ny][nx] != '#' and person_dist[ny][nx] == -1:
                next_time = curr_time + 1
                # 불보다 먼저 도착해야 함
                if next_time < fire_dist[ny][nx]:
                    person_dist[ny][nx] = next_time
                    person_queue.append((ny, nx))

    if escaped_time == -1:
        print("IMPOSSIBLE")
    else:
        print(escaped_time)