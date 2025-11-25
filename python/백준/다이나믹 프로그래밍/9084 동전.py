import sys

input = sys.stdin.readline

T = int(input())


for _ in range(T):

    N = int(input())

    coins = list(map(int, input().split()))

    target = int(input())
    d = [0] * (target+1)
    d[0] = 1

    for coin in coins:
        for money in range(coin, target + 1):
            d[money] += d[money - coin]

    print(d[target])