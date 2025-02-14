from collections import deque

A, B = map(int, input().split())

def bfs(A, B):
    queue = deque([(A, 1)])
    
    while queue:
        position, count = queue.popleft()
    
        if position == B:
            return count
        else:
            new_position = position * 2
            if new_position <= B:
                count += 1
                queue.append((new_position, count))
            
            if new_position <= B:
                new_position = int(str(position) + '1')
                queue.append((new_position,count))
    return -1

print(bfs(A, B))