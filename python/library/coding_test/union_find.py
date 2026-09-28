"""유니온 파인드: 집합 합치기/같은 집합 판별. 크루스칼: 최소 신장 트리."""


class UnionFind:
    def __init__(self, n):
        self.parent = list(range(n + 1))
        self.size = [1] * (n + 1)

    def find(self, node):
        # 경로 압축: 조상을 건너뛰며 트리를 낮춘다. 재귀 없이 구현.
        while node != self.parent[node]:
            self.parent[node] = self.parent[self.parent[node]]
            node = self.parent[node]
        return node

    def union(self, a, b):
        """합쳐졌으면 True, 이미 같은 집합이면 False. 연산당 상각 O(α(N))."""
        a, b = self.find(a), self.find(b)
        if a == b:
            return False

        # 작은 트리를 큰 트리에 붙인다.
        if self.size[a] < self.size[b]:
            a, b = b, a
        self.parent[b] = a
        self.size[a] += self.size[b]
        return True


def kruskal(n, edges):
    """정점 1~n(n >= 1), 무방향 edges = [(비용, 정점A, 정점B), ...].

    최소 신장 트리의 비용을 반환한다. 연결 불가능하면 None. O(E log E).
    최단 경로가 아니라 모든 정점을 연결하는 총 간선 비용을 최소화한다.
    """
    uf = UnionFind(n)
    total = 0
    selected = 0

    for cost, a, b in sorted(edges):
        if uf.union(a, b):  # 이미 연결된 정점을 잇는 간선은 사이클을 만든다.
            total += cost
            selected += 1
            if selected == n - 1:
                break

    return total if selected == n - 1 else None


if __name__ == "__main__":
    uf = UnionFind(4)
    uf.union(1, 2)
    print(uf.find(1) == uf.find(2))  # True
    print(uf.find(1) == uf.find(3))  # False
    edges = [(1, 1, 2), (4, 1, 3), (2, 2, 3), (3, 3, 4)]
    print(kruskal(4, edges))  # 6
