import sys

input = sys.stdin.readline

N = int(input().strip())

# 첫 번째 행을 초기화
a, b, c = map(int, input().split())
max_a, max_b, max_c = a, b, c
min_a, min_b, min_c = a, b, c

for _ in range(N - 1):
    a, b, c = map(int, input().split())

    # 최댓값 갱신
    new_max_a = max(max_a, max_b) + a
    new_max_b = max(max_a, max_b, max_c) + b
    new_max_c = max(max_b, max_c) + c

    # 최솟값 갱신
    new_min_a = min(min_a, min_b) + a
    new_min_b = min(min_a, min_b, min_c) + b
    new_min_c = min(min_b, min_c) + c

    # 업데이트
    max_a, max_b, max_c = new_max_a, new_max_b, new_max_c
    min_a, min_b, min_c = new_min_a, new_min_b, new_min_c

# 최종 결과 출력
print(max(max_a, max_b, max_c), min(min_a, min_b, min_c))