import sys

input = sys.stdin.readline

cursor = list(input())
stack = []

n = int(input())

for _ in range(n):
    command = input().strip().split()
    
    if command[0] == 'L':
        if cursor:
            stack.append(cursor.pop())
    
    elif command[0] == 'D':
        if stack:
            cursor.append(stack.pop())
    
    elif command[0] == 'B':
        if cursor:
            cursor.pop()
    
    elif command[0] == 'P':
        cursor.append(command[1])


print(''.join(cursor) + ''.join(stack))
    
    
