import sys

input = sys.stdin.readline

n = int(input())

dp = [0] * 10001

wine = []
wine.append(0)

for i in range(1, n + 1):
    wine.append(int(input()))


if n == 1:
    print(wine[1])
    sys.exit()
elif n == 2:
    print(wine[1] + wine[2])
    sys.exit()


dp[1] = wine[1]
dp[2] = wine[1] + wine[2]
dp[3] = max(wine[3] + wine[2], wine[3] + wine[1])
for i in range(4, n + 1):

    candidate1 = wine[i] + wine[i-1] + dp[i-3]
    candidate2 = wine[i] + dp[i-2]

    if candidate1 > 33:
        candidate1 = 0
    if candidate2 > 33:
        candidate2 = 0

    
    if candidate1 == 33 or candidate2 == 33:
        print(33)
        exit(0)

    dp[i] = max(candidate2, candidate1, dp[i-1])

print(max(dp))