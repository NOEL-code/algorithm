string = input()
bomb = input()

stack = []
bomb_len = len(bomb)

for ch in string:
    stack.append(ch)
    # 폭탄 문자열 길이만큼 스택 끝부분이 폭탄이랑 같으면 제거
    if ''.join(stack[-bomb_len:]) == bomb:
        for _ in range(bomb_len):
            stack.pop()

result = ''.join(stack)
print(result if result else "FRULA")