import sys

input = sys.stdin.readline

N, M = map(int, input().split())

data = list(map(int, input().split()))

sum = [0]

temp = 0

for i in data:
  temp += i
  sum.append(temp)
  
for _ in range(M):
  i, j = map(int, input().split())
  
  print(sum[j] - sum[i -1])
  