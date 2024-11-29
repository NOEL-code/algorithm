n, m, k = map(int, input().split())

data = list(map(int, input().split()))

data.sort(reverse=True)

result = 0

largest = data[0]
small = data[1]

while True:
  for i in range(k):
    if m == 0:
      break
    result =+ largest
    m = m - 1
  if m == 0:
    break
  result =+ small
  m = m - 1
  
print(result)