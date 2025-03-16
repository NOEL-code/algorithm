from collections import deque

MAX_POSITION = 100000
N, K = map(int, input().split())

def bfs(N, K):
    visited = [-1] * (MAX_POSITION + 1)  # -1로 초기화 (방문 여부 & 시간 저장)
    queue = deque()
    queue.append(N)
    visited[N] = 0  # 시작 위치 시간 = 0

    while queue:
        current = queue.popleft()

        if current == K:  # 동생 찾으면 시간 반환
            return visited[current]

        # 1️⃣ 순간이동 (시간 증가 없음): 큐 앞에 추가
        if 0 <= current * 2 <= MAX_POSITION and visited[current * 2] == -1:
            visited[current * 2] = visited[current]  # 시간 증가 X
            queue.appendleft(current * 2)


        # 2️⃣ 걷기: X - 1
        if 0 <= current - 1 <= MAX_POSITION and visited[current - 1] == -1:
            visited[current - 1] = visited[current] + 1
            queue.append(current - 1)

        # 3️⃣ 걷기: X + 1
        if 0 <= current + 1 <= MAX_POSITION and visited[current + 1] == -1:
            visited[current + 1] = visited[current] + 1
            queue.append(current + 1)

print(bfs(N, K))