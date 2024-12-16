from collections import deque

circle = deque()

n, k = map(int, input().split())

for i in range(1, n + 1):
  circle.append(i)

result = []
while len(circle) > 0:
  for _ in range(k - 1):
    circle.append(circle.popleft())
  result.append(circle.popleft())
  
print("<", end='')
print(", ".join(map(str, result)), end='')
print(">")