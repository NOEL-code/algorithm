import sys
import heapq

input = sys.stdin.readline
INF = int(1e9)

T = int(input())

for _ in range(T):
    n, d, c = map(int, input().split())
    
    graph = [[] for _ in range(n + 1)]
    distance = [INF] * (n + 1)
    
    for _ in range(d):
        a, b, s = map(int, input().split())
        graph[b].append((a, s))
    
    def dijkstra(start):
        queue = []
        heapq.heappush(queue, (0, start))
        distance[start] = 0
        
        while queue:
            dist, now = heapq.heappop(queue)
            
            if dist > distance[now]:
                continue
            
            for next_node, cost in graph[now]:
                new_cost = dist + cost
                
                if new_cost < distance[next_node]:
                    distance[next_node] = new_cost
                    heapq.heappush(queue, (new_cost, next_node))
    
    dijkstra(c)
    
    # 🔥 결과 계산
    count = 0
    max_time = 0
    
    for i in range(1, n + 1):
        if distance[i] != INF:
            count += 1
            max_time = max(max_time, distance[i])
    
    print(count, max_time)