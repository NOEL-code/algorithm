"""다익스트라: 음수 간선이 없는 가중 그래프의 한 출발점 최단 거리.

힙 구현은 O((V + E) log(E + 2)), 일반적인 단순 그래프에서는 O((V + E) log V).
graph[u] = [(도착 정점, 비용), ...], 정점 번호는 1~N.
"""

import heapq


def dijkstra(graph, start):
    distance = [float("inf")] * len(graph)
    distance[start] = 0
    heap = [(0, start)]  # 반드시 (거리, 정점) 순서: 거리 기준 최소 힙

    while heap:
        current_dist, node = heapq.heappop(heap)

        # 더 짧은 경로가 이미 발견된 오래된 항목은 무시한다.
        if current_dist > distance[node]:
            continue

        for next_node, weight in graph[node]:
            new_dist = current_dist + weight
            if new_dist < distance[next_node]:
                distance[next_node] = new_dist
                heapq.heappush(heap, (new_dist, next_node))

    return distance


if __name__ == "__main__":
    # 방향 그래프. 무방향이라면 반대 방향 간선도 추가한다.
    graph = [[], [(2, 2), (3, 5)], [(3, 1), (4, 4)], [(4, 1)], [], []]
    print(dijkstra(graph, 1)[1:])  # [0, 2, 3, 4, inf]
    # 음수 간선이 있으면 이 구현을 사용하면 안 된다.
