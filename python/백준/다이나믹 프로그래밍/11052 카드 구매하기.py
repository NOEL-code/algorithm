import sys

input = sys.stdin.readline

n = int(input())
card_cost = list(map(int, input().split()))

d = [0] * (n+1)

d[1] = card_cost[0]
for i in range(1, n+1):

    d[i] = max(card_cost[i-1], d[i-1] + d[1])

print(d[i])