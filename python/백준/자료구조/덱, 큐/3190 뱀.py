import sys
from collections import deque

input = sys.stdin.readline

N = int(input())
K = int(input())

apples = []

for _ in range(K):
    x, y = map(int, input().split())
    apples.append((x, y))
    
L = int(input())

commands = []

for _ in range(L):
    second, direction = map(str, input().split())
    commands.append((int(second), direction))
    
answer = 0

directions = [(0, 1), (1, 0), (0, -1), (-1, 0)]

def change_direction(current_direction, next_direction):
    if next_direction == 'D':
        return (current_direction + 1) % 4
    elif next_direction == 'L':
        return (current_direction - 1) % 4
        
snake = deque([(1, 1)])
current_direction = 0

while True:
    answer += 1
    
    current_head_x, current_head_y = snake[-1]
    
    next_head_x, next_head_y = current_head_x + directions[current_direction][0], current_head_y + directions[current_direction][1]
    
    if (next_head_x, next_head_y) in snake:
        break
    
    if 0 < next_head_x <= N and 0 < next_head_y <= N:
        snake.append((next_head_x, next_head_y))
    else:
        break
    
    if (next_head_x, next_head_y) in apples:
        apples.remove((next_head_x, next_head_y))
    else:
        snake.popleft()
        
    if commands:
        if commands[0][0] == answer:
            current_direction = change_direction(current_direction, commands[0][1])
            commands.pop(0)
            
print(answer)