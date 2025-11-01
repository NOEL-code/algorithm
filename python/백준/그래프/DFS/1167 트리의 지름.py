def dfs(node, dist):
    for next_node, cost in graph[node]:
        if visited[next_node] == -1:
            visited[next_node] = dist + cost
            dfs(next_node, dist + cost)
            
import sys
sys.setrecursionlimit(10**6)
input = sys.stdin.readline  
         
N = int(input())

graph = [[] for _ in range(N+1)]
for _ in range(N):
    tree = list(map(int, input().split()))
    for i in range(1, len(tree)//2):
        graph[tree[0]].append((tree[2*i-1], tree[2*i]))
            

       
visited = [-1] * (N+1)
visited[1] = 0       
dfs(1, 0)
max_dist = max(visited)
max_node = visited.index(max_dist)


visited = [-1] * (N+1)
visited[max_node] = 0
dfs(max_node, 0)

print(max(visited))