n = int(input())

result = 0

for i in range((n + 1) * 60 * 60):
  h = i // 3600
  m = (i % 3600) // 60
  s = i % 60
  
  if '3' in str(h) + str(m) + str(s):
    result += 1

print(result)