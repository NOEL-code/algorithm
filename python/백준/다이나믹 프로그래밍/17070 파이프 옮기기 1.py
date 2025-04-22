import sys
input = sys.stdin.readline

N = int(input())
graph = [list(map(int, input().split())) for _ in range(N)]

dp = [[[0]*3 for _ in range(N)] for _ in range(N)]

# 초기 위치: (0,1)까지 가로 파이프
dp[0][1][0] = 1

for x in range(N):
    for y in range(2, N):  # (0,0)과 (0,1)은 이미 초기 조건이므로 y는 2부터 시작
        if graph[x][y] == 1:
            continue
        
        # 가로
        dp[x][y][0] = dp[x][y-1][0] + dp[x][y-1][2] if graph[x][y-1] == 0 else 0

        # 세로
        if x > 0 and graph[x-1][y] == 0:
            dp[x][y][1] = dp[x-1][y][1] + dp[x-1][y][2]

        # 대각선
        if x > 0 and graph[x][y-1] == 0 and graph[x-1][y] == 0 and graph[x-1][y-1] == 0:
            dp[x][y][2] = dp[x-1][y-1][0] + dp[x-1][y-1][1] + dp[x-1][y-1][2]

print(sum(dp[N-1][N-1]))  # 도착점 (N-1, N-1)에 도달하는 모든 방향의 경우의 수 합