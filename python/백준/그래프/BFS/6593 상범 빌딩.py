import sys
from collections import deque

input = sys.stdin.readline

while True:
    L, R, C = map(int, input().split())

    if L == 0 and R == 0 and C == 0:
        sys.exit(0)

    building = [list(map(str, input().split())) for _ in range(L)]
