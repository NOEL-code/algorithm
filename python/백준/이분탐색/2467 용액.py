import sys

input = sys.stdin.readline

N = int(input().strip())
nums = list(map(int, input().split()))

left, right = 0, N - 1
best_sum = float('inf')
a, b = nums[left], nums[right]

while left < right:
    s = nums[left] + nums[right]

    if abs(s) < abs(best_sum):
        best_sum = s
        a, b = nums[left], nums[right]
        if best_sum == 0:  # 더 이상 좋아질 수 없음
            break

    if s > 0:
        right -= 1
    else:
        left += 1

# 오름차순 출력
if a > b:
    a, b = b, a
print(a, b)