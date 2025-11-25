import sys
input = sys.stdin.readline

N = int(input())
L = list(map(int, input().split()))  # 잃는 체력
J = list(map(int, input().split()))  # 얻는 기쁨

MAX_LOSS = 99  # 총 잃을 수 있는 체력의 최대값

dp = [0] * (MAX_LOSS + 1)  # dp[h] = 체력을 h만큼 잃었을 때의 최대 기쁨

for i in range(N):
    lose = L[i]
    joy = J[i]
    # 0/1 배낭: 내림차순으로 순회
    for h in range(MAX_LOSS, lose - 1, -1):
        dp[h] = max(dp[h], dp[h - lose] + joy)

print(max(dp))  