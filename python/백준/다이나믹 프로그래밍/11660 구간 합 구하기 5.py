import sys

input = sys.stdin.readline

N, M = map(int, input().split())

array = [list(map(int, input().split())) for _ in range(N)]
request = [list(map(int, input().split())) for _ in range(M)]

# **누적합(dp) 배열 생성 (0부터 시작하는 padding 포함)**
dp = [[0] * (N + 1) for _ in range(N + 1)]


# **누적합 계산**
for i in range(1, N + 1):
    for j in range(1, N + 1):
        dp[i][j] = array[i-1][j-1] + dp[i-1][j] + dp[i][j-1] - dp[i-1][j-1]

# **쿼리 처리**
for x1, y1, x2, y2 in request:
    result = dp[x2][y2] - dp[x1-1][y2] - dp[x2][y1-1] + dp[x1-1][y1-1]
    print(result)