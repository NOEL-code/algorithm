import sys

input = sys.stdin.readline

N = int(input())

building = list(map(int, input().split()))

left = 0
right = 0
answer = 0


for i in range(N):
    