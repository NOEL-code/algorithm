import sys
input = sys.stdin.readline

n, b = input().split()

array = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

n = n[::-1]
result = 0

for i, n in enumerate(n):
  result += (int(b) ** i) * (array.index(n))
  
print(result)