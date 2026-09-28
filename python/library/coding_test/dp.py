"""DP: 상태의 의미 → 점화식 → 초기값 → 계산 순서를 정한다."""

from bisect import bisect_left


def min_coins(coins, target):
    """양의 정수 동전을 무제한 사용해 target(0 이상)을 만드는 최소 개수.

    dp[amount] = amount원을 만드는 최소 동전 개수.
    불가능하면 -1. O(len(coins) * target), 공간 O(target).
    """
    dp = [float("inf")] * (target + 1)
    dp[0] = 0

    for amount in range(1, target + 1):
        for coin in coins:
            if coin <= amount:
                dp[amount] = min(dp[amount], dp[amount - coin] + 1)

    return dp[target] if dp[target] != float("inf") else -1


def knapsack(items, capacity):
    """0/1 배낭: 각 물건은 최대 한 번 사용. items = [(양의 무게, 가치), ...].

    dp[limit] = 무게 합이 limit 이하일 때 최대 가치.
    capacity >= 0. O(N * capacity), 공간 O(capacity).
    """
    dp = [0] * (capacity + 1)
    for weight, value in items:
        # 역순이어야 같은 물건을 이번 반복에서 재사용하지 않는다.
        for limit in range(capacity, weight - 1, -1):
            dp[limit] = max(dp[limit], dp[limit - weight] + value)
    return dp[capacity]


def lis_length(values):
    """엄격하게 증가하는 최장 부분 수열(LIS)의 길이. O(N log N).

    tails[i] = 길이가 i+1인 증가 부분 수열의 마지막 값 중 최솟값.
    tails 자체가 원본 배열의 실제 LIS인 것은 아니다.
    """
    tails = []
    for value in values:
        index = bisect_left(tails, value)
        if index == len(tails):
            tails.append(value)
        else:
            tails[index] = value
    return len(tails)


if __name__ == "__main__":
    print(min_coins([1, 3, 4], 6))  # 2: 3 + 3 (큰 동전 우선 그리디는 실패)
    print(knapsack([(6, 13), (4, 8), (3, 6), (5, 12)], 7))  # 14
    print(lis_length([10, 20, 10, 30, 20, 50]))  # 4
