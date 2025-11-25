N, K = map(int, input().split())

num = input()

stack = []
answer = []
count = 0
for digit in num:

    while stack and stack[-1] < int(digit) and count < K:
        stack.pop()
        count += 1

    stack.append(int(digit))

if count < K:
    for _ in range(K-count):
        stack.pop()

for i in range(len(stack)):
    print(stack[i], end="")