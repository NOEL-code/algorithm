import sys

input = sys.stdin.readline
sys.setrecursionlimit(10**7)

N = int(input())
M = int(input())

parent = [0] * N
for i in range(N):
    parent[i] = i

def find_parent(parent, x):
    if parent[x] != x:
        parent[x] = find_parent(parent, parent[x])
    else:
        return parent[x]

def union_parent(parent, a, b):
    a = find_parent(parent, a)
    b = find_parent(parent, b)
    
    if a < b:
        parent[b] = a
    else:
        parent[a] = b

for _ in range(M):
    oper, a, b = map(int, input().split())
    
    if oper == 1:
        union_parent(parent, a, b)

a, b, c = map(int, input().split())

if find_parent(parent, a) == find_parent(parent, b) and find_parent(parent, b) == find_parent(parent, c):
        print("YES")
else:
        print("NO")