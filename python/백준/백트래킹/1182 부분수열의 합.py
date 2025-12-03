import sys
from itertools import combinations
input = sys.stdin.readline

N, S = map(int, input().split())

nums = list(map(int, input().split()))
visited_index = [False] * N

count = 0

for i in range(1, N +1):
    comb = combinations(nums, i)

    for x in comb:
        if sum(x) == S:
            count  += 1
print(count)