import sys
input = sys.stdin.readline

n = int(input().rstrip())

for _ in range(n):
    string = input().rstrip()
    stack = []
    flag = True  # 올바른 괄호 문자열인지 확인하는 플래그
    for i in range(len(string)):
        if string[i] == '(':
            stack.append('(')
        elif string[i] == ')':
            if not stack or stack[-1] != '(':
                flag = False  # `==` 대신 `=` 사용
                break
            else:
                stack.pop()
    if flag and not stack:  # 스택이 비어 있어야 올바른 문자열
        print("YES")
    else:
        print("NO")