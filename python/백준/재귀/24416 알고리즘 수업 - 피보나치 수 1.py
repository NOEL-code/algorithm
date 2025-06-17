n = int(input())

dp = [0] * (n + 1)
dp[1] = 1
dp[2] = 1
count_dp = 0

for i in range(3, n + 1):
    dp[i] = dp[i - 1] + dp[i - 2]
    count_dp += 1

print(dp[n], n - 2)