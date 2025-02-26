import sys

input = sys.stdin.readline

N, W = map(int, input().split())

value = []
weight = []
dp = [[0] * (W + 1) for _ in range(N + 1)]

for _ in range(N):
    w, v = map(int, input().split())
    weight.append(w)
    value.append(v)

for i in range(1, N + 1):
    for w in range(1, W + 1):
        if weight[i - 1] > w:
            dp[i][w] = dp[i - 1][w]
        else:
            dp[i][w] = max(dp[i - 1][w], value[i - 1] + dp[i - 1][w - weight[i - 1]]) 

print(dp[N][W])            
    
