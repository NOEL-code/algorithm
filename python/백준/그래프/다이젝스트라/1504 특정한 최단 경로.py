import sys
import heapq

input = sys.stdin.readline
INF = int(1e9)

N, E = map(int, input().split())  # 정점 개수, 간선 개수

graph = [[] for _ in range(N + 1)]  # 인접 리스트 생성

# 그래프 입력
for _ in range(E):
    a, b, c = map(int, input().split())
    graph[a].append((b, c))
    graph[b].append((a, c))

# 반드시 거쳐야 하는 두 개의 정점
base1, base2 = map(int, input().split())

def dijkstra(start):
    """ 다익스트라 알고리즘을 사용하여 start에서 모든 정점까지의 최단 거리 계산 """
    distance = [INF] * (N + 1)
    queue = []
    
    heapq.heappush(queue, (0, start))  # (비용, 현재 노드)
    distance[start] = 0  # 시작점의 거리는 0
    
    while queue:
        dist, now = heapq.heappop(queue)  # 현재까지의 거리, 현재 노드
        
        if distance[now] < dist:
            continue  # 이미 처리된 경우 스킵
        
        for next_node, next_dist in graph[now]:
            cost = dist + next_dist
            if cost < distance[next_node]:  # 더 짧은 경로 발견
                distance[next_node] = cost
                heapq.heappush(queue, (cost, next_node))
    
    return distance

# 1. 1번 노드에서 다익스트라 실행
distance_from_start = dijkstra(1)

# 2. base1에서 다익스트라 실행
distance_from_base1 = dijkstra(base1)

# 3. base2에서 다익스트라 실행
distance_from_base2 = dijkstra(base2)

# 4. 두 가지 경로를 계산
path1 = distance_from_start[base1] + distance_from_base1[base2] + distance_from_base2[N]  # 1 -> base1 -> base2 -> N
path2 = distance_from_start[base2] + distance_from_base2[base1] + distance_from_base1[N]  # 1 -> base2 -> base1 -> N

# 5. 도달 불가능한 경로 예외 처리
result = min(path1, path2)
print(result if result < INF else -1)