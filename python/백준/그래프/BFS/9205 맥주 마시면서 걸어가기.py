import sys
from collections import deque

input = sys.stdin.readline

T = int(input())

for _ in range(T):
    n = int(input())
    
    home_x, home_y = map(int, input().split())
    
    conveniences = []
    
    for _ in range(n):
        convenience_x, convenience_y = map(int, input().split())
        conveniences.append((convenience_x, convenience_y, False))
    
    festival_x, festival_y = map(int, input().split())
    
    queue = deque([(home_x, home_y, 20)])
    
    while queue:
        x, y, beers = queue.popleft()

        if x == festival_x and y == festival_y:
            print("happy")
            sys.exit(0)
            
        
    
    
    