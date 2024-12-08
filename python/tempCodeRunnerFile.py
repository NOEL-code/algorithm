import sys
input = sys.stdin.readline

n = int(input().strip())

d = [0] * 41
zero = [0] * 41
one = [0] * 41

zero[0] = 1
zero[1] = 0
zero[2] = 1
zero[3] = 1

one[0] = 0
one[1] = 1
one[2] = 1
one[3] = 2

def fibo(x):

    if x == 0:
        return 0
    elif x == 1:
        return 1
    else:
        if d[x] != 0:
            return d[x]
        else:
            d[x] = fibo(x - 1) + fibo(x - 2)
            zero[x] = zero[x - 1] + zero[x - 2]
            one[x] = one[x - 1] + one[x - 2]
            return d[x]
        
for i in range(n):

    x = int(input())
    
    result = fibo(x)
    
    print(zero[x], one[x])
    
    zero = [0] * 41
    one = [0] * 41
    
    