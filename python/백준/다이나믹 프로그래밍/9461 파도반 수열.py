T = int(input())

d = [0] * 101

d[1] = 1
d[2] = 1
d[3] = 1
d[4] = 2
d[5] = 2
 
for _ in range(T):
    N = int(input())
    
    if N < 6:
        print(d[N])
    else:
        for i in range(6, N + 1):
            d[i] = d[i - 5] + d[i - 1]

        print(d[N])