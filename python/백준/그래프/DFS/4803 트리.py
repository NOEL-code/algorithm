import sys
input = sys.stdin.readline
sys.setrecursionlimit(10**6)

case = 1
while True:
    n, m = map(int, input().split())
    if n == 0 and m == 0:
        break

    graph = [[] for _ in range(n + 1)]
    for _ in range(m):
        a, b = map(int, input().split())
        graph[a].append(b)
        graph[b].append(a)

    visited = [False] * (n + 1)

    def check_cycle(node, parent):
        for neighbor in graph[node]:
            if neighbor == parent:
                continue
            if visited[neighbor]:
                return True          # 부모가 아닌 방문 정점이면 사이클
            visited[neighbor] = True
            if check_cycle(neighbor, node):
                return True
        return False

    tree_count = 0
    for i in range(1, n + 1):
        if not visited[i]:
            visited[i] = True
            if not check_cycle(i, 0):
                tree_count += 1

    if tree_count == 0:
        print(f"Case {case}: No trees.")
    elif tree_count == 1:
        print(f"Case {case}: There is one tree.")
    else:
        print(f"Case {case}: A forest of {tree_count} trees.")
    case += 1