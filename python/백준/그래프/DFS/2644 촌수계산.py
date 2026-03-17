import sys
sys.setrecursionlimit(10**6)

input = sys.stdin.readline

n = int(input())
start, end = map(int, input().split())

graph = [[] for _ in range(n+1)]

m = int(input())

for _ in range(m):
    a, b = map(int, input().split())
    graph[a].append(b)
    graph[b].append(a)

visited = [-1] * (n + 1)

def dfs(node, dist):
    if node == end:
        return dist
    
    for next_node in graph[node]:
        if visited[next_node] == -1:
            visited[next_node] = dist + 1
            result = dfs(next_node, dist + 1)
            if result != -1:
                return result
    
    return -1

visited[start] = 0
print(dfs(start, 0))