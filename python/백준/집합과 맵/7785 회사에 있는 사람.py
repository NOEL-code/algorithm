n = int(input())

_dict = {}

for i in range(n):
  name, command = input().rstrip().split()

  if command == "enter":
    _dict[name] = True
  else:
    del _dict[name]
  
result = sorted(_dict.keys(), reverse=True)

for name in result:
  print(name)