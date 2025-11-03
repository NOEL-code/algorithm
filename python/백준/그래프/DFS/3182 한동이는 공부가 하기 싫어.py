import sys
input = sys.stdin.readline

n = int(input())
graph = [0]
result = [0]

for i in range(1, n+1):
    graph.append(int(input()))

def dfs(start):
    visited[start] = True

    if not visited[graph[start]]:
        dfs(graph[start])

for i in range(1, n+1):
    visited = [False] * (n+1)
    dfs(i)
    result.append(visited.count(True))

print(result.index(max(result)))
