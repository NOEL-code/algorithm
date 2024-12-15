n, k = map(int, input().split())

array = []
for _ in range(n):
    array.append(int(input()))

array.sort()

print(array[0])