import sys
import heapq

input = sys.stdin.readline

N, M, X = map(int, input().split())

INF = int(1e9)
graph = [[] for _ in range(N + 1)]
reverse_graph = [[] for _ in range(N + 1)]

for _ in range(M):
    a, b, cost = map(int, input().split())
    graph[a].append((b, cost))
    reverse_graph[b].append((a, cost))  # 방향 뒤집기

def dijkstra(start, graph):
    distance = [INF] * (N + 1)
    
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
                
    return distance

# X → i
dist_from_X = dijkstra(X, graph)

# i → X (뒤집어서 X → i)
dist_to_X = dijkstra(X, reverse_graph)

ans = 0
for i in range(1, N + 1):
    ans = max(ans, dist_to_X[i] + dist_from_X[i])

print(ans)