from collections import deque

MAX_POSITION = 100000
N, K = map(int, input().split())

graph = []
visited = [False] * (MAX_POSITION + 1)
result = []
def bfs(N, K, count):
    if N == K:
        return 0
    
    queue = deque([(N, count)])
    visited[N] = True
    
    while queue:
        current_position, count = queue.popleft()
        
        for next_position in (current_position * 2, current_position - 1, current_position + 1):
            if 0 <= next_position <= MAX_POSITION and visited[next_position] == False:
                if next_position != current_position * 2:
                    count += 1
                visited[next_position] = True
                queue.append((next_position, count))
            
                if next_position == K:
                    return count

print(bfs(N, K, 0))




