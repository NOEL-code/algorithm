import sys
sys.setrecursionlimit(10**6)
input = sys.stdin.readline

N, M, R = map(int, input().split())

graph = [[] for _ in range(N+1)]
visited = [0] * (N+1)

for _ in range(M):  # M개 간선 읽기
    a, b = map(int, input().split())
    graph[a].append(b)
    graph[b].append(a)

for i in range(1, N+1):   # 각 인접리스트 오름차순 정렬
    graph[i].sort()

order = 1                 # 방문 순서 카운터

def dfs(start):
    global order
    for neighbor in graph[start]:
        if visited[neighbor] == 0:
            order += 1
            visited[neighbor] = order   # 방문 '순서' 기록
            dfs(neighbor)

visited[R] = order  # 시작 정점 방문 순서 = 1
dfs(R)

for i in range(1, N+1):
    print(visited[i])