import sys
from collections import deque

input = sys.stdin.readline

T = int(input())

for _ in range(T):
    N, M = map(int, input().split())
    temp_list = list(map(int, input().split()))
    
    queue = deque([])
    for i, doc in enumerate(temp_list):
        queue.append((doc, i))    
        
    count = 0
    while queue:
        current = queue.popleft()
        if any (p > current[0] for p, _ in queue):
            queue.append(current)
        else:
            count += 1
            if current[1] == M:
                print(count)
                break
