a, b, c = map(int, input().split())

data = list(map(int, input().split()))

data.sort()
data.reverse()

result = 0

while(True):
    for i in range(c):
        if b == 0:
            break
        result += data[0]
        b -= 1
    if b == 0:
        break
    result += data[1]
    b -= 1
    
print(result)    