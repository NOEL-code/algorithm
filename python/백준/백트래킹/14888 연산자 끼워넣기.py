import sys
input = sys.stdin.readline

N = int(input())
nums = list(map(int, input().split()))
operand = list(map(int, input().split()))  # +, -, *, /

min_num = 1e9
max_num = -1e9

def dfs(idx, total, plus, minus, multiply, divide):
    global min_num, max_num
    
    if idx == N:
        max_num = max(total, max_num)
        min_num = min(total, min_num)
        return

    if plus > 0:
        dfs(idx + 1, total + nums[idx], plus - 1, minus, multiply, divide)
    if minus > 0:
        dfs(idx + 1, total - nums[idx], plus, minus - 1, multiply, divide)
    if multiply > 0:
        dfs(idx + 1, total * nums[idx], plus, minus, multiply - 1, divide)
    if divide > 0:
        dfs(idx + 1, int(total / nums[idx]), plus, minus, multiply, divide - 1)

dfs(1, nums[0], operand[0], operand[1], operand[2], operand[3])
print(max_num)
print(min_num)