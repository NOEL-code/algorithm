n = int(input())

cards = list(map(int, input().split(" ")))

m = int(input())

checks = list(map(int, input().split()))

_dict = {}

for i in range(len(cards)):
  _dict[cards[i]] = 0 # 아무 값이나 매핑한 거임
  

for j in range(m):
  if checks[j] not in _dict:
    print(0, end=" ")
  else:
    print(1, end=' ')
    


import sys

n = int(sys.stdin.readline())

cards = list(map(int, sys.stdin.readline().split()))

m = int(sys.stdin.readline())

checks = list(map(int, sys.stdin.readline().split()))
 
cards.sort()

for check in checks:
  start, end = 0, n - 1
  exist = False
  
  while start <= end:
    mid = (start + end) // 2
    if cards[mid] == check:
      exist = True
      break
    elif cards[mid] > check:
      end = mid - 1
    else:
      start = mid + 1
  
  print(1 if exist else 0, end=' ')
    