n = int(input())

array_a = list(map(int, input().split()))
array_b = list(map(int, input().split()))

array_a.sort()
array_b.sort(reverse=True)

sum = 0
for i in range(n):
    sum += array_b[i] * array_a[i] 

print(sum)