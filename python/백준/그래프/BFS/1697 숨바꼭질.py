from collections import deque

MAX_POSITION = 100000
visited = [-1] * (MAX_POSITION + 1)

def bfs(N, K):
    if N == K:
        return 0
    
    queue = deque([N])
    visited[N] = 0
    
    while queue:
        current_position = queue.popleft()
        
        for next_position in (current_position - 1, current_position * 2, current_position + 1):
            if 0 <= next_position <= MAX_POSITION and visited[next_position] == -1:
                visited[next_position] = visited[current_position] + 1
                queue.append(next_position)
            
                if next_position == K:
                    return visited[next_position]
            

N, K = map(int, input().split())        

print(bfs(N, K))