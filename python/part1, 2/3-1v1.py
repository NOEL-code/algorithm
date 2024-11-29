a, b = map(int, input().split())

result = []

for i in range(a):
    data = list(map(int, input().split()))
    
    result.append(min(data))

result.sort()
result.reverse()

print(result[0])
    