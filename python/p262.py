import heapq
import sys

input = sys.stdin.readline
INF = int(1e9)

n, m, start = map(int, input().split())

# 그래프를 인접 리스트로 초기화
graph = [[] for i in range(n + 1)]

# 거리 테이블을 무한으로 초기화
distance = [INF] * (n + 1)

# 모든 간선 정보 입력
for _ in range(m):
  x, y, z = map(int, input().split())
  graph[x].append((y, z))

# 다익스트라 알고리즘 함수
def dijkstra(start):
  q = []
  heapq.heappush(q, (0, start))
  distance[start] = 0
  while q:
    dist, now = heapq.heappop(q)
    if distance[now] < dist:
      continue
    for i in graph[now]:
      cost = dist + i[1]
      if cost < distance[i[0]]:
        distance[i[0]] = cost
        heapq.heappush(q, (cost, i[0]))

# 다익스트라 알고리즘 수행
dijkstra(start)

# 도달할 수 있는 노드의 개수
count = 0
# 도달할 수 있는 노드 중 가장 멀리 있는 노드와의 최단 거리
max_distance = 0

for d in distance:
  if d != INF:
    count += 1
    max_distance = max(max_distance, d)

# 시작 노드는 제외해야 하므로 count - 1 출력
print(count - 1, max_distance)
