def find_parent(parent, x):
  if parent[x] != x:
    parent[x] = find_parent(parent, parent[x])
  return parent[x]

def union_parent(parent, a, b):
  a = find_parent(a)
  b = find_parent(b)
  
  if a < b:
    parent[b] = a
  else:
    parent[a] = b
  
v, e = map(int, input().split())

parent = [0] * (v+ 1)

edges = []
result = 0

for i in range(1, v + 1):
  parent[i] = i