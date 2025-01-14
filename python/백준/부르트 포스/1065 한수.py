n = int(input())

count = 0

for i in range(1, n + 1):  # n을 포함하도록 범위 수정
    s = str(i)
    if len(s) == 1:  # 한 자리 수는 모두 한수
        count += 1
    else:
        # 등차수열 확인
        dif = int(s[0]) - int(s[1])
        flag = True
        for j in range(1, len(s)):
            if int(s[j - 1]) - int(s[j]) != dif:
                flag = False
                break
        if flag:
            count += 1

print(count)
