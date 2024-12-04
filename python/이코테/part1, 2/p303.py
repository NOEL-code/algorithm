from collections import deque
import copy
v = int(input())

graph = [[] for _ in range(v + 1)]

indegree = [0] * (v + 1)

time = [0] * (v + 1)
for i in range(1, v + 1):
  a = int(input())
  
  while a != -1:
    indegree[i] += 1
    graph[i].append[a]
    

def topology_sort():    
  result = copy.deepcopy(time)
  q = deque()

  for i in range(1, v + 1):
    if indegree[i] == 0:
      q.append(i)

  while q:
    now = q.popleft()
  
    for i in graph[now]:
      indegree[i] -= 1
      result[i] += max(result[i], result[now] + time[i])
      if indegree[i] == 0:
        q.append(i)
  for i in range(1, v + 1):      
    print(result[i])
    
topology_sort()