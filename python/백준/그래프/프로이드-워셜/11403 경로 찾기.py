import sys

input = sys.stdin.readline

N = int(input())

graph = [[0] * N for _ in range(N)]

for i in range(N):
    row = list(map(int, input().split()))
    for j in range(N):
        graph[i][j] = row[j]

for k in range(N):
    for i in range(N):
        for j in range(N):
            if graph[i][k] == 1 and graph[k][j] == 1:
                graph[i][j] = 1
            
for row in graph:
    print(*row)
    