import sys

input = sys.stdin.readline

N = int(input())

stack = []
stack.append(int(input()))

for _ in range(N-1):
    num = int(input())
    while stack and stack[-1] <= num:
        stack.pop()
    
    stack.append(num)

print(len(stack))