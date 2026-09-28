"""DFS: 연결 여부, 연결 요소, 모든 경로 탐색의 기본. O(V + E).

정점 번호는 1~N, graph[0]은 사용하지 않는다.
깊은 그래프에서는 재귀 대신 스택 버전을 사용한다.
"""


def dfs_recursive(graph, start):
    visited = [False] * len(graph)
    order = []

    def dfs(node):
        visited[node] = True
        order.append(node)

        for next_node in graph[node]:
            if not visited[next_node]:
                dfs(next_node)

    dfs(start)
    return order


def dfs_stack(graph, start):
    visited = [False] * len(graph)
    stack = [start]
    order = []

    while stack:
        node = stack.pop()
        # 여러 경로로 같은 정점이 스택에 들어올 수 있다.
        if visited[node]:
            continue

        visited[node] = True
        order.append(node)

        # 스택은 LIFO: 역순으로 넣어야 재귀 DFS와 같은 순서로 방문한다.
        for next_node in reversed(graph[node]):
            if not visited[next_node]:
                stack.append(next_node)

    return order


def count_components(graph):
    """무방향 그래프의 연결 요소 개수. 고립된 정점도 하나로 센다."""
    visited = [False] * len(graph)
    count = 0

    for start in range(1, len(graph)):
        if visited[start]:
            continue

        count += 1
        visited[start] = True
        stack = [start]

        while stack:
            node = stack.pop()
            for next_node in graph[node]:
                if not visited[next_node]:
                    visited[next_node] = True
                    stack.append(next_node)

    return count


if __name__ == "__main__":
    # 무방향 간선은 양쪽 인접 리스트에 넣는다. 6번 정점은 고립되어 있다.
    graph = [[], [2, 3], [1, 4, 5], [1], [2], [2], []]
    print(dfs_recursive(graph, 1))  # [1, 2, 4, 5, 3]
    print(dfs_stack(graph, 1))      # [1, 2, 4, 5, 3]
    print(count_components(graph))  # 2
