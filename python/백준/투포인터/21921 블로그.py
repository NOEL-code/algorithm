import sys
input = sys.stdin.readline

N, X = map(int, input().split())
income = list(map(int, input().split()))

# 처음 X일(0 ~ X-1)의 합
current_sum = sum(income[0:X])
max_income = current_sum
max_count = 1

# X일부터 N-1일까지 창문을 한 칸씩 오른쪽으로 밀기
for i in range(X, N):
    # 창문에서 왼쪽 값 하나 빼고, 새로 들어온 오른쪽 값 하나 더하기
    current_sum += income[i] - income[i - X]

    if current_sum == max_income:
        max_count += 1
    elif current_sum > max_income:
        max_income = current_sum
        max_count = 1

if max_income == 0:
    print("SAD")
else:
    print(max_income)
    print(max_count)