import heapq
import sys

input = sys.stdin.readline
INF = int(1e9)

n, m, r = map(int, input().split())
items = list(map(int, input().split()))  # 각 지역 아이템 수

# 그래프 초기화
graph = [[] for _ in range(n)]
for _ in range(r):
    a, b, l = map(int, input().split())
    graph[a-1].append((b-1, l))
    graph[b-1].append((a-1, l))  # 양방향 연결

# 다익스트라 알고리즘
def dijkstra(start):
    q = []
    distance = [INF] * n
    distance[start] = 0
    heapq.heappush(q, (0, start))

    while q:
        dist, now = heapq.heappop(q)
        if distance[now] < dist:
            continue
        for neighbor, weight in graph[now]:
            cost = dist + weight
            if cost < distance[neighbor]:
                distance[neighbor] = cost
                heapq.heappush(q, (cost, neighbor))

    # 수색 범위 m 이내 아이템 총합
    total_items = 0
    for i in range(n):
        if distance[i] <= m:
            total_items += items[i]
    return total_items

# 시작점을 바꿔가며 최대 아이템 수 탐색
result = 0
for i in range(n):
    result = max(result, dijkstra(i))

print(result)