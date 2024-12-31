import sys
input = sys.stdin.readline

n = int(input())

data = []

for _ in range(n):
    a, b = map(int, input().split())
    data.append((a, b))

data.sort(key = lambda x: (x[1], x[0]))

end = data[0][1]
count = 1

for i in range(1, n):
    if data[i][0] >= end:
        end = data[i][1]
        count += 1

print(count)  