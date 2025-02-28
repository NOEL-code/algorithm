import sys
import heapq
 
input = sys.stdin.readline

V, E = map(int, input().split())
start = int(input())

INF = 1e9

graph = [[] for _ in range(V + 1)]
distance = [INF] * (V + 1)

for _ in range(E):
    a, b, cost = map(int, input().split())
    
    graph[a].append((b, cost))

def dijkstra(start):
    queue = []
    heapq.heappush(queue, (0, start))
    distance[start] = 0
    
    while queue:
        dist, now = heapq.heappop(queue)
        
        if dist > distance[now]:
            continue
        
        for next_node, next_dist in graph[now]:
            cost = dist + next_dist
            
            if cost < distance[next_node]:
                distance[next_node] = cost
                heapq.heappush(queue, (cost, next_node))

dijkstra(start)

for i in range(1, V + 1):
    if distance[i] == INF:
        print("INF")
    else:
        print(distance[i])