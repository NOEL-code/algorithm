N = int(input())

tower = list(map(int, input().split()))
tower_list = []

for idx, height in enumerate(tower):
    tower_list.append((idx+1, height))

stack = []
answer = []

for t_idx, t_height in tower_list:
    while stack and stack[-1][1] < t_height:
        stack.pop()
        
    if stack and stack[0][1] >= t_height:
        answer.append(stack[-1][0])
    else:
        answer.append(0)
        
    stack.append((t_idx, t_height))
    
for i in range(len(answer)):
    print(answer[i], end=" ")