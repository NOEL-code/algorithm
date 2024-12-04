import sys
input = sys.stdin.readline

n = int(input())

data = []
for i in range(n):
    age, name = input().split()
    data.append([int(age), name])

s_array = sorted(data, key=lambda x: x[0])

for i in range(n):
    print(s_array[i][0], s_array[i][1])

