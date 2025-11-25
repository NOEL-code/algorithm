import sys

input = sys.stdin.readline

n = int(input())
# 인덱스를 1부터 쓰기 위해 앞에 0 하나 추가
card_cost = [0] + list(map(int, input().split()))

d = [0] * (n + 1)

for i in range(1, n + 1):
    for j in range(1, i + 1):
        # i장을 만들기 위해 j장짜리 카드팩을 마지막으로 하나 사용하는 경우
        d[i] = max(d[i], d[i - j] + card_cost[j])

print(d[n])
