n = int(input())

big5 = n // 5
n = n % 5
small3 = 0
isPossible = True

while(True):
  small3 = n // 3
  
  if n % 3 != 0 and big5 == 0:
    isPossible = False
    break
  
  if n % 3 != 0:
    big5 -= 1
    n += 5
    continue
  
  break

if isPossible:
  print(big5 + small3)
else:
  print(-1)