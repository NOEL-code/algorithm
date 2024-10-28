def find_team(team, x):
  if team[x] != x:
    team[x] = find_team(team, team[x])
  return team[x]

  
def union_team(team, a, b):
  
  a = find_team(team, a)
  b = find_team(team, b)
  
  if a < b:
    team[b] = a
  else:
    team[a] = b
    
def check_same_team(team, a, b):
  if find_team(team, a) == find_team(team, b):
    return True
  else:
    return False

n, m = map(int, input().split())
  
team = [0] * (n + 1)

for i in range(1, n + 1):
  team[i] = i
  
for _ in range(m):
  operation, a, b = map(int, input().split())
  
  if operation == 0:
    if check_same_team(team, a, b):
      continue
    else:
      union_team(team, a, b)
    
  else:
    if check_same_team(team, a, b):
      print("YES")
    else:
      print("NO")

