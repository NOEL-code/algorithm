from collections import deque

T = int(input())

for _ in range(T):
    n = int(input())
    
    home = tuple(map(int, input().split()))
    conv = [tuple(map(int, input().split())) for _ in range(n)]
    festival = tuple(map(int, input().split()))
    
    queue = deque([home])
    visited = [False] * n
    
    is_happy = False
    
    while queue:
        x, y = queue.popleft()
        
        if abs(x - festival[0]) + abs(y - festival[1]) <= 1000:
            print("happy")
            is_happy = True
            break
        
        for i in range(n):
            if not visited[i]:
                nx, ny = conv[i]
                
                if abs(x - nx) + abs(y - ny) <= 1000:
                    visited[i] = True
                    queue.append((nx, ny))
    
    if not is_happy:
        print("sad")