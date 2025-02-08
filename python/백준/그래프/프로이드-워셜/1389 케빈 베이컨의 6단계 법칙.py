import sys

input = sys.stdin.readline
N, M = map(int, input().split())

INF = 1e9

graph = [[INF] * (N + 1) for _ in range(N + 1)]

for a in range(1, N + 1):
    for b in range(1, N + 1):
        if a == b:
            graph[a][b] = 0
            
for _ in range(M):
    a, b = map(int, input().split())
    
    graph[a][b] = 1
    graph[b][a] = 1

for k in range(1, N + 1):
    for a in range(1, N + 1):
        for b in range(1, N + 1):
            graph[a][b] = min(graph[a][b], graph[a][k] + graph[k][b])

count_array = [0] * (N + 1)

for a in range(1, N + 1):
    for b in range(1, N + 1):
        if graph[a][b] != INF:
            count_array[a] += graph[a][b]
   
min_value = min(count_array[1:])  # 1번부터 N번까지 최소값 찾기
min_index = count_array.index(min_value)  # 최소값을 가진 인덱스 찾기
print(min_index)