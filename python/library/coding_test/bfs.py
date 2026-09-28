"""BFS: 모든 간선 비용이 같을 때 최단 거리. 그래프 O(V + E), 격자 O(NM)."""

from collections import deque


def bfs(graph, start):
    """정점 1~N. 시작점 거리는 0, 도달할 수 없는 정점은 -1."""
    distance = [-1] * len(graph)
    distance[start] = 0
    queue = deque([start])

    while queue:
        node = queue.popleft()  # list.pop(0)은 O(N)이므로 사용하지 않는다.
        for next_node in graph[node]:
            if distance[next_node] == -1:
                # 꺼낼 때가 아니라 넣을 때 방문 처리하여 중복 삽입을 막는다.
                distance[next_node] = distance[node] + 1
                queue.append(next_node)

    return distance


def grid_bfs(board, start, end):
    """비어 있지 않은 직사각형 격자. 1=이동 가능, 0=벽. 좌표는 (행, 열).

    반환값은 최소 이동 횟수(시작 칸 제외), 도달 불가능하면 -1.
    start와 end는 격자 범위 안의 좌표여야 한다. board는 수정하지 않는다.
    """
    rows, cols = len(board), len(board[0])
    sr, sc = start
    er, ec = end
    if board[sr][sc] == 0 or board[er][ec] == 0:
        return -1

    distance = [[-1] * cols for _ in range(rows)]
    distance[sr][sc] = 0
    queue = deque([(sr, sc)])
    directions = [(1, 0), (-1, 0), (0, 1), (0, -1)]

    while queue:
        row, col = queue.popleft()
        if (row, col) == end:
            return distance[row][col]

        for dr, dc in directions:
            nr, nc = row + dr, col + dc
            if 0 <= nr < rows and 0 <= nc < cols:
                if board[nr][nc] == 1 and distance[nr][nc] == -1:
                    distance[nr][nc] = distance[row][col] + 1
                    queue.append((nr, nc))

    return -1


if __name__ == "__main__":
    graph = [[], [2, 3], [1, 4], [1], [2], []]
    print(bfs(graph, 1)[1:])  # [0, 1, 1, 2, -1]

    board = [[1, 0, 1], [1, 1, 1], [0, 0, 1]]
    print(grid_bfs(board, (0, 0), (2, 2)))  # 4
    # 시작 칸도 세는 문제라면 도달 가능한 결과에만 1을 더한다.
