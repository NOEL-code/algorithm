import sys
input = sys.stdin.readline

n = int(input())

# 계단 점수 입력받기
steps = [0] * (n + 1)
for i in range(1, n + 1):
    steps[i] = int(input())

# DP 테이블 초기화
d = [0] * (n + 1)

# n이 1이나 2일 때 예외 처리
if n == 1:
    print(steps[1])
    sys.exit()
elif n == 2:
    print(steps[1] + steps[2])
    sys.exit()

# 초기값 설정
d[1] = steps[1]
d[2] = steps[1] + steps[2]
d[3] = max(steps[3] + steps[2], steps[3] + steps[1])

# 점화식 적용
for i in range(4, n + 1):
    d[i] = max(d[i - 3] + steps[i - 1] + steps[i], d[i - 2] + steps[i])

# 결과 출력
print(d[n])