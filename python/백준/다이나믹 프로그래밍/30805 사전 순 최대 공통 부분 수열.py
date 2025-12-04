from itertools import combinations

N = int(input())
A = list(map(int, input().split()))

M = int(input())
B = list(map(int, input().split()))

common = []

for a in A:
    if a in B:
        common.append(a)

for i in range(1, len(common) + 1):
    for x in combinations(common, i):
        print(x)
