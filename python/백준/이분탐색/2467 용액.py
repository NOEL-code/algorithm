N = int(input())
nums = list(map(int, input().split()))
nums.sort()

min_num = [0, 0, int(1e9)]  # [값1, 값2, 현재 최솟값(절댓값)]

def search(left, right):
    while left < right:
        s = nums[left] + nums[right]

        # 최솟값 갱신 (절댓값 기준)
        if abs(s) < abs(min_num[2]):
            min_num[0], min_num[1], min_num[2] = nums[left], nums[right], abs(s)

        if s == 0:
            print(nums[left], nums[right])
            return True  # 정확히 0이면 바로 종료

        if s > 0:
            right -= 1
        else:
            left += 1
    return False

# 이 문제는 한 번만 돌리면 됨
found = search(0, N-1)
if not found:
    print(min_num[0], min_num[1])