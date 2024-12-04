n = int(input())

data = list(map(int, input().split()))

data.sort()

result = 0
count = 0

for fear in data:
  count += 1
  if count >= fear:
    result += 1
    count = 0

print(result)
