import sys
input = sys.stdin.readline
sys.setrecursionlimit(10**6)

def find_parent(parent, x):
    if parent[x] != x:
        parent[x] = find_parent(parent, parent[x])  # 경로 압축
    return parent[x]

def union_parent(parent, a, b):  # ✅ 함수명 수정
    a = find_parent(parent, a)
    b = find_parent(parent, b)
    
    if a < b:
        parent[b] = a
    else:
        parent[a] = b

V, E = map(int, input().split())

parent = [0] * (V + 1)
edges = []
result = 0

for _ in range(E):
    a, b, cost = map(int, input().split())
    edges.append((cost, a, b))

# 각 노드를 자기 자신으로 초기화
for i in range(1, V + 1):
    parent[i] = i

# 간선 비용 기준 정렬
edges.sort()

for cost, a, b in edges:
    # 사이클이 발생하지 않으면 MST에 포함
    if find_parent(parent, a) != find_parent(parent, b):
        union_parent(parent, a, b)
        result += cost

print(result)