import sys
input = sys.stdin.readline

N, R, Q = map(int, input().split())
graph = [[] for _ in range(N + 1)]
    
for _ in range(N - 1):
    u, v = map(int, input().split())
    graph[u].append(v)
    graph[v].append(u)

# 서브트리 크기 계산 (반복 DFS)
size = [1] * (N + 1)
parent = [0] * (N + 1)
order = []  # 방문 순서 기록
visited = [False] * (N + 1)

stack = [R]
visited[R] = True
parent[R] = 0

while stack:
    node = stack.pop()
    order.append(node)
    for nxt in graph[node]:
        if not visited[nxt]:
            visited[nxt] = True
            parent[nxt] = node
            stack.append(nxt)

# 방문 순서의 역순으로 자식 크기를 부모에게 더함
for node in reversed(order):
    if parent[node] != 0:
        size[parent[node]] += size[node]

# 쿼리 처리
out = []
for _ in range(Q):
    u = int(input())
    out.append(str(size[u]))

print('\n'.join(out) + '\n')

