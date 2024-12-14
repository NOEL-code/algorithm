a, b, v = map(int, input().split())

result = 0

days = (v - b) // (a - b)

if ((v - b) % (a - b)) != 0:
  days += 1
   
print(days) 