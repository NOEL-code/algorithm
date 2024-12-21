n = int(input())

array = []
for _ in range(n):
    array.append(n)
    
array.sort()

print(array[0] * n)