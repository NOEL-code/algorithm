
import sys
input = sys.stdin.readline

n = int(input())

data = []
for i in range(n):
    x, y = map(int, input().split())
    data.append([y, x])

s_array = sorted(data)

for i in range(n):
    print(s_array[i][1], s_array[i][0])

