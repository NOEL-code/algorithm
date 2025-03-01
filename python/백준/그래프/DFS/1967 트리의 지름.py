import sys
sys.setrecursionlimit(10**6)
input = sys.stdin.readline

# 입력 받기
n = int(input())

# 그래프 초기화
graph = [[] for _ in range(n + 1)]

# 간선 정보 입력
for _ in range(n - 1):
    a, b, c = map(int, input().split())
    graph[a].append((b, c))
    graph[b].append((a, c))

# 가장 먼 노드를 찾는 함수
def dfs(node, dist):
    global max_dist, farthest_node
    if dist > max_dist:
        max_dist = dist
        farthest_node = node

    for next_node, cost in graph[node]:
        if visited[next_node] == -1:  # 방문하지 않은 노드
            visited[next_node] = dist + cost
            dfs(next_node, dist + cost)

# 1. 첫 번째 DFS 실행 (임의의 노드: 1번에서 가장 먼 노드 찾기)
visited = [-1] * (n + 1)
max_dist = 0
farthest_node = 1
visited[1] = 0
dfs(1, 0)

# 2. 두 번째 DFS 실행 (가장 먼 노드에서 시작해서 최대 거리 찾기)
visited = [-1] * (n + 1)
max_dist = 0
visited[farthest_node] = 0
dfs(farthest_node, 0)

# 트리의 지름 출력
print(max_dist)