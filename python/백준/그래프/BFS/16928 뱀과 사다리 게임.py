import sys
from collections import deque 
input = sys.stdin.readline

N, M = map(int, input().split())

ladder = {}
snake = {}

for _ in range(N):
    start, end = map(int, input().split())
    ladder[start] = end
    
for _ in range(M):
    start, end = map(int, input().split())
    snake[start] = end

    
def bfs():
    queue = deque([1])
    visited = [-1] * 101
    visited[1] = 0
    
    while queue:
        node = queue.popleft()
        
        if node == 100:
            return visited[node]
        
        for dice_num in range(1, 7):
            next_node = node + dice_num
                
            if next_node > 100:
                continue
            
            if next_node in ladder:
                next_node = ladder[next_node]
                
            elif next_node in snake:
                next_node = snake[next_node]
                    
            if visited[next_node] == -1:
                visited[next_node] = visited[node] + 1
                queue.append(next_node)
    
    return -1

print(bfs())