import sys
import heapq

input = sys.stdin.readline

M, N = map(int, input().split())
board = [list(map(int, input().rstrip())) for _ in range(N)]

INF = int(1e9)

distance = [[INF] * M for _ in range(N)]  # 수정: N행 M열
distance[0][0] = 0

directions = [(0, 1), (1, 0), (-1, 0), (0, -1)]
queue = []
heapq.heappush(queue, (0, 0, 0))

while queue:
    cost, y, x = heapq.heappop(queue)
    
    if y == N - 1 and x == M - 1:
        print(cost)
        sys.exit()
    
    if cost > distance[y][x]:
        continue
    
    for dy, dx in directions:
        ny, nx = y + dy, x + dx
        
        if not (0 <= ny < N and 0 <= nx < M):
            continue
        
        new_cost = cost + board[ny][nx]
        
        if new_cost < distance[ny][nx]:
            distance[ny][nx] = new_cost
            heapq.heappush(queue, (new_cost, ny, nx))  # 수정: ny, nx