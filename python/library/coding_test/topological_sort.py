"""위상 정렬: 선행 조건이 있는 작업의 순서. 방향 그래프, O(V + E)."""

from collections import deque


def topological_sort(graph):
    """정점 1~N. 가능한 순서 하나를 반환하며, 사이클이 있으면 None."""
    n = len(graph) - 1
    indegree = [0] * (n + 1)
    for node in range(1, n + 1):
        for next_node in graph[node]:
            indegree[next_node] += 1

    queue = deque(node for node in range(1, n + 1) if indegree[node] == 0)
    order = []

    while queue:
        node = queue.popleft()
        order.append(node)
        for next_node in graph[node]:
            indegree[next_node] -= 1
            if indegree[next_node] == 0:
                queue.append(next_node)

    # 사이클에 속하거나 그에 의존하는 정점은 처리되지 않는다.
    return order if len(order) == n else None


if __name__ == "__main__":
    graph = [[], [3], [3], [4], []]
    print(topological_sort(graph))  # [1, 2, 3, 4]
    print(topological_sort([[], [2], [1]]))  # None: 1 → 2 → 1
    # 매번 가능한 가장 작은 번호를 선택해야 한다면 deque 대신 heapq 사용.
