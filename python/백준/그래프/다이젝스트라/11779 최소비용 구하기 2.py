import sys
import heapq

input = sys.stdin.readline

INF = int(1e9)

N = int(input())
M = int(input())

graph = [[] for _ in range(N + 1)]
distance = [INF] * (N + 1)
prev = [-1] * (N + 1)
for _ in range(M):
    a, b, c = map(int, input().split())
    
    graph[a].append((b, c))

start, end = map(int, input().split())

def dijkstra(start):
    distance[start] = 0
    queue = [(0, start)]
    
    while queue:
        dist, now = heapq.heappop(queue)
        
        if distance[now] < dist:
            continue
        
        for next_node, next_cost in graph[now]:
            cost = dist + next_cost
            if distance[next_node] > cost:
                distance[next_node] = cost
                prev[next_node] = now
                heapq.heappush(queue, (cost, next_node))
    
dijkstra(start)
print(distance[end])

path = []
current = end
while current != -1:
    path.append(current)
    current = prev[current]

path.reverse()

print(len(path))
print(*path)