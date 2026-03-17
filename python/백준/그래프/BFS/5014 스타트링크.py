from collections import deque
import sys


F, S, G, U, D = map(int, input().split())

MAX_POSITION = F

visited = [-1] * (MAX_POSITION + 1)
visited[S] = 0

def bfs():
    if S == G:
        print(0)
        sys.exit(0)
    
    queue = deque([S])
    
    while queue:
        current_posistion = queue.popleft()
        if current_posistion == G:
            return
        next_posistion_U = current_posistion + U
        next_posistion_D = current_posistion - D
        
        if 0 < next_posistion_U <= MAX_POSITION and visited[next_posistion_U] == -1:
            visited[next_posistion_U] = visited[current_posistion] + 1
            queue.append(next_posistion_U)

        if 0 < next_posistion_D <= MAX_POSITION and visited[next_posistion_D] == -1:
            visited[next_posistion_D] = visited[current_posistion] + 1
            queue.append(next_posistion_D)

bfs()


if visited[G] == -1:
    print("use the stairs")
else:
    print(visited[G])