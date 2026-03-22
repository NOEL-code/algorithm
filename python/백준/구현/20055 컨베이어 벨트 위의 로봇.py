N, K = map(int, input().split())

belt = list(map(int, input().split()))

robots = [False] * N  

step = 0

while True:
    step += 1 
    
    belt = [belt[-1]] + belt[:-1]
    robots = [False] + robots[:-1]
    robots[-1] = False
    
    for i in range(N - 2, -1, -1):
        if robots[i] and not robots[i + 1] and belt[i + 1] > 0:
            robots[i] = False
            robots[i + 1] = True
            belt[i + 1] -= 1
    
    robots[-1] = False
    
    if belt[0] > 0:
        robots[0] = True
        belt[0] -= 1
    
    if belt.count(0) >= K:
        break
    
print(step)