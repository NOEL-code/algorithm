n = int(input())

array = list(map(int, input().split()))
result = [-1] * n
stack = []

stack.append(0)

for i in range(1, n):
    while stack and array[stack[-1]] < array[i]:
        result[stack.pop()] = array[i]
        
    stack.append(i)
    
    
print(*result)z