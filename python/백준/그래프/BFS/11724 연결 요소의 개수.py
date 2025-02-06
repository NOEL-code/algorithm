from collections import deque
N, M = map(int, input().split())

graph = [[] for _ in range(N + 1)]
visited = [False] * (N + 1)

for _ in range(M):
    u, v = map(int, input().split())
    
    graph[u].append(v)
    graph[v].append(u)

def bfs(start):
    queue = deque([start])    
    visited[start] = True
    
    while queue:
        node = queue.popleft()
        
        for neighbor in graph[node]:
            if not visited[neighbor]:
                visited[neighbor] = True
                queue.append(neighbor)

count = 0
for i in range(1, N + 1):
    if visited[i]:
        continue
    bfs(i)
    count += 1
    
print(count)



N, M = map(int, input().split())

graph = [[] for _ in range(N + 1)]
visited = [False] * (N + 1)

for _ in range(M):
    u, v = map(int, input().split())
    graph[u].append(v)
    graph[v].append(u)

def dfs(node):
    visited[node] = True
    for neighbor in graph[node]:
        if not visited[neighbor]:
            dfs(neighbor)

count = 0
for i in range(1, N + 1):
    if not visited[i]:  # 방문하지 않은 경우에만 DFS 호출
        dfs(i)
        count += 1  # 새로운 연결 요소 발견 시 증가

print(count)