n = int(input())

d = [0] * 30001

data = list(map(int, input().split()))

for i in range(1, n):
  d[i] = max(d[i -1], d[i - 2] + data[i])

print(d[n - 1])