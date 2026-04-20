import sys
input = sys.stdin.readline

N = int(input())
nums = list(map(int, input().split()))
nums.sort()

best = [0, 0, 0]
best_abs = float('inf')

for i in range(N - 2):
    left, right = i + 1, N - 1
    
    while left < right:
        s = nums[i] + nums[left] + nums[right]
        
        if abs(s) < best_abs:
            best = [nums[i], nums[left], nums[right]]
            best_abs = abs(s)
        
        if s == 0:
            print(*best)
            sys.exit()
        elif s > 0:
            right -= 1
        else:
            left += 1

print(*best)