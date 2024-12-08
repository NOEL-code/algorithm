import sys
input = sys.stdin.readline

n = int(input().strip())

d = [0] * 41
zero = [0] * 41
one = [0] * 41

# 초기값 설정
d[0], d[1] = 0, 1
zero[0], one[0] = 1, 0
zero[1], one[1] = 0, 1

for i in range(2, 41):
    d[i] = d[i - 1]+ d[i - 2]
    zero[i] = zero[i - 1]+ zero[i - 2]
    one[i] = one[i - 1]+ one[i - 2]
    
for i in range(n):
    
    x = int(input())
    
    print(zero[x], one[x])
    
    