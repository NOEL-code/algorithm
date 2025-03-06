import sys
import heapq

input = sys.stdin.readline
INF = (1e9)
N, E = map(int, input().split())

graph = [[] for _ in range(N + 1)]

for _ in range(E):
    a, b, c = map(int, input().split())
    graph[a].append((b, c))
    graph[b].append((a, c))

base1, base2 = map(int, input().split())

def dijkstra(start):
    queue = []
    
    heapq.heappush(queue, (0, start))
    
    while queue:
        dist, now = heapq.heappop(queue)
        
        if dist > distance[now]:
            continue
        
        for next_node, next_dist in graph[now]:
            cost = dist + next_dist
            
            if cost < distance[next_node]:
                distance[next_node] = cost
                heapq.heappush(queue, (cost, next_node))
            
distance = [INF] * (N + 1)
dijkstra(1)
stage1 = distance[base1]

distance = [INF] * (N + 1)
dijkstra(base1)
stage2 = distance[base2]

distance = [INF] * (N + 1)
dijkstra(base2)
stage3 = distance[N]

candidate1 = stage1 + stage2 + stage3

distance = [INF] * (N + 1)
dijkstra(1)
stage1 = distance[base2]

distance = [INF] * (N + 1)
dijkstra(base2)
stage2 = distance[base1]

distance = [INF] * (N + 1)
dijkstra(base1)
stage3 = distance[N]

candidate2 = stage1 + stage2 + stage3

if min(candidate1, candidate2) == INF:
    print(-1)
else:
    print(min(candidate1, candidate2))
