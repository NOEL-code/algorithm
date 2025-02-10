import sys

input = sys.stdin.readline

N = int(input())

graph = [list(map(int, input().rstrip().split())) for _ in range(N)]
d = [[0] * 3 for _ in range(N)]

d[0] = graph[0]
    
for i in range(1, N):
    d[i][0] = min(d[i - 1][1], d[i - 1][2]) + graph[i][0]
    d[i][1] = min(d[i - 1][0], d[i - 1][2]) + graph[i][1]
    d[i][2] = min(d[i - 1][1], d[i - 1][0]) + graph[i][2]
    
print(min(d[N - 1]))