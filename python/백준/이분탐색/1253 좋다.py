import sys
input = sys.stdin.readline

N = int(input())
nums = list(map(int, input().split()))
nums.sort()

def search(start, end, target):
    while start <= end:
        mid = (start + end) // 2
        if nums[mid] == target:
            return True
        elif nums[mid] > target:
            end = mid - 1
        else:
            start = mid + 1
    return False

count = 0
for i in range(0, N - 2):              # i: 0..N-3
    for j in range(i + 1, N - 1):      # j: i+1..N-2
        if search(j + 1, N - 1, nums[i] + nums[j]):  # k > j
            count += 1

print(count)