import sys
input = sys.stdin.readline

T = int(input())
for _ in range(T):
    N, M = map(int, input().split())
    for _ in range(M):
        input()  # 간선 입력은 소모만 하면 됨
    print(N - 1)