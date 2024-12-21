n = int(input())

array = []
max = 0
for _ in range(n):
    array.append(int(input()))

array.sort()

for i in range(n):
    if max <= array[i] * (n - i):
        max = array[i] * (n - i)

print(max)