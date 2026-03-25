T = int(input())

for _ in range(T):
    n = int(input())
    
    dp = [[0] * 4 for _ in range(n + 1)]

    dp[0][1] = 1
    dp[0][2] = 1  
    dp[0][3] = 1

    for i in range(1, n + 1):
        dp[i][1] = 1
        
        for j in range(2, 4):  # j=1은 위에서 처리했으니 2부터
            if i - j >= 0:
                dp[i][j] = dp[i - j][j] + dp[i][j - 1]
            else:
                dp[i][j] = dp[i][j - 1]

    print(dp[n][3])