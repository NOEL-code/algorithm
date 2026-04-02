import sys
input = sys.stdin.readline

N, M = map(int, input().split())

edges = []
for _ in range(M):
    A, B, C = map(int, input().split())
    edges.append((A, B, C))

INF = int(1e9)
dist = [INF] * (N + 1)
dist[1] = 0

# N-1번 반복
for i in range(N - 1):
    for u, v, w in edges:
        if dist[u] != INF and dist[u] + w < dist[v]:
            dist[v] = dist[u] + w

# 음수 사이클 체크
for u, v, w in edges:
    if dist[u] != INF and dist[u] + w < dist[v]:
        print(-1)
        exit()

# 결과 출력
for i in range(2, N + 1):
    if dist[i] == INF:
        print(-1)
    else:
        print(dist[i])