import sys
import heapq

input = sys.stdin.readline
INF = int(1e9)

# 도시 수 (노드), 버스 수 (간선)
N = int(input().strip())
M = int(input().strip())

# 그래프 초기화 (1-indexed)
graph = [[] for _ in range(N + 1)]
distance = [INF] * (N + 1)

# 버스 정보 입력
for _ in range(M):
    a, b, cost = map(int, input().split())
    graph[a].append((b, cost))  # 단방향 그래프

# 다익스트라 함수
def dijkstra(start):
    distance[start] = 0
    q = [(0, start)]  # (비용, 노드)

    while q:
        dist, now = heapq.heappop(q)
        if distance[now] < dist:
            continue  # 이미 방문한 노드면 스킵

        for next_node, next_cost in graph[now]:
            cost = dist + next_cost
            if cost < distance[next_node]:  # 더 짧은 경로 발견 시 갱신
                distance[next_node] = cost
                heapq.heappush(q, (cost, next_node))

# 구간 정보 입력 (출발 도시, 도착 도시)
start, end = map(int, input().split())

# 최단 거리 계산 및 출력
dijkstra(start)
print(distance[end])