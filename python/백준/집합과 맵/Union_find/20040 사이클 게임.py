def find_parent(parent, x):
    if parent[x] != x:
        parent[x] = find_parent(parent, parent[x])
    return parent[x]
    
def union_parent(parent, a, b):
    a = find_parent(parent, a)
    b = find_parent(parent, b)
    

    if a == b:
        return True
        
    if a < b:
        parent[b] = a
    else:
        parent[a] = b
        
import sys

input = sys.stdin.readline

N, M = map(int, input().split())

parent = [0] * N
for i in range(N):
    parent[i] = i


for i in range(1, M + 1):
    a, b = map(int, input().split())
    if union_parent(parent, a, b):
        print(i)
        sys.exit(0)

print(0)