n , m = map(int, input().split())

parent = [0] * (n + 1)

for i in range(n + 1):
  parent[i] = i

def find_parent(parent, x):
  if parent[x] != x:
    find_parent(parent, parent[x])
  return parent[x]
                
def union_parent(parent, a, b):
  a = find_parent(parent, a)
  b = find_parent(parent, b)
  if a < b:
    parent[b] = a
  else:
    parent[a] = b
      
              
for _ in range(m):
  oper, a, b = map(int, input().split())
  
  if oper == 1:
    if find_parent(parent, a) == find_parent(parent, b):
      print("YES")
    else:
      print("NO")
  else:
    union_parent(a, b)