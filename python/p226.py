n, m = map(int, input().split())

data = []

d = [0] * 10001

for i in range(n):
  data.append(int(input()))

data.sort()

for i in data:
  d[i] = 1

for i in range(data[0], m + 1):
    
  for j in range(data[1], len(data)):
    d[i] = min(d[i - data[j]] + 1, d[i - data[j - 1]] + 1)
    print(i, d[i])

    
if d[m] == 0:
  print('-1')
else:
  print(d[m])

  

  
  