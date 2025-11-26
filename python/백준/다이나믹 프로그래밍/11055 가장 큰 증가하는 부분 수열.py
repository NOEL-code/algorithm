N = int(input())
array = list(map(int, input().split()))

d = [0] * N

for i in range(N):
    d[i] = array[i]

for i in range(N):
    for j in range(i):
        if array[i] > array[j]:
            d[i] = max(d[i], array[i] + d[j])
            
print(max(d))