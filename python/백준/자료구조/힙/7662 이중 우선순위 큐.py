import heapq
import sys

input = sys.stdin.readline


T = int(input())

for _ in range(T):
    Q = int(input())
    min_queue = []
    max_queue = []
    visited = [False] * Q
    
    for i in range(Q):
        command, num = input().split()
        
        if command == "I":
            heapq.heappush(min_queue, (int(num), i))
            heapq.heappush(max_queue, (-int(num), i))
            
        elif command == "D":    
            if int(num) == -1:
                while min_queue:
                    num, index = heapq.heappop(min_queue)
                    if visited[index]:
                        continue
                    else:
                        visited[index] = True
                        break
                        
            elif int(num) == 1:
                while max_queue:
                    num, index = heapq.heappop(max_queue)
                    if visited[index]:
                        continue
                    else:
                        visited[index] = True
                        break
 
    if all(visited):
        print("EMPTY")
    else:
        max_value = None
        while max_queue:
            num, index = heapq.heappop(max_queue)
            
            if not visited[index]:
                max_value = -num
                break
        
        min_value = None
        while min_queue:
            num, index = heapq.heappop(min_queue)
            
            if not visited[index]:
                min_value = num
                break
        
        if max_value is not None and min_value is not None:
            print(max_value, min_value)
        else:
            print("EMPTY")
            