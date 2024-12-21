from collections import deque

n, k = map(int, input().split())

q = deque()

for i in range(1, n + 1):
  q.append(i)

result = []

while q:
  q.rotate(-(k- 1))
  result.append(q.popleft())

print("<", end="")
print(", ".join(map(str, result)), end="")
print(">")