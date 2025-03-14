import heapq
import sys

input = sys.stdin.readline

INF = int(1e9)

n, m ,start = map(int, input().split())

graph = [[] for _ in range(n + 1)]

distance = [INF] * (n + 1)

for i in range(m):
  a, b, c = map(int, input().split())
  graph[a].append((b, c))

def dijkstra(start):
  q = []
  heapq.heappush(q, (0, start))
  distance[start] = 0
  
  while q:
    dist, now = heapq.heappop()
    
    if distance[now] < dist:
      continue
    
    for i in graph[now]:
      cost = dist + i[1]
      
      if cost < distance[i[0]]:
        distance[i[0]] = cost
        heapq.heappush(q, (cost, i[0]))
        
dijkstra(start)

max_time = 0
count = 0

for i in range(n + 1):
  if distance[i] == INF:
    continue
  else:
    count += 1
    max_time = max(max_time, distance[i])

print(count - 1, max_time)
    