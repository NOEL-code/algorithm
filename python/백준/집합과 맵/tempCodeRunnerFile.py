n, m = map(int, input().split())

_dict = {}
for i in range(1, n + 1):
  name = input().rstrip()
  _dict[name] = i
  _dict[i] = name

for i in range(m):
  search = input().rstrip()
  
  if search.isdigit():
    print(_dict[int(search)])
  else:
    print(_dict[search])
  