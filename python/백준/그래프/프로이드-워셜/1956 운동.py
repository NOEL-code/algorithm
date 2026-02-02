import sys

input = sys.stdin.readline
V, E = map(int, input().split())

INF= int(1e9)

graph = [[INF] * (V + 1) for _ in range(V + 1)]

for i in range(E):
    a, b, c = map(int, input().split())
    
    graph[a][b] = c
            
for k in range(1, V + 1):
    for a in range(1, V + 1):
        for b in range(1, V + 1):
            graph[a][b] = min(graph[a][k] + graph[k][b], graph[a][b])

answer = INF

for i in range(1, V + 1):
    for j in range(1, V + 1):
        answer = min(graph[i][j] + graph[j][i], answer)

if answer == INF:
    print(-1)
else:
    print(answer)