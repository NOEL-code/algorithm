import sys

input = sys.stdin.readline

n = int(input())

straw = []
for _ in range(n):
    straw.append(int(input()))

straw.sort(reverse=True)


for i in range(0, n-2):
    a, b, c = straw[i], straw[i+1], straw[i+2]

    if a < b + c:
        print(a+b+c)
        exit(0)

print(-1)