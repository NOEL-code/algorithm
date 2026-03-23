import sys
from itertools import combinations

input = sys.stdin.readline

N, M = map(int, input().split())

board = [list(map(int, input().split())) for _ in range(N)]

chickens = []

home_count = 0
homes = []

for i in range(N):
    for j in range(N):
        if board[i][j] == 2:
            chickens.append((i, j))

        elif board[i][j] == 1:
            home_count += 1
            homes.append((i, j))

def calculate_distance(home, chickens):
    home_x, home_y = home
    
    min_distance = int(1e9)
    
    for chicken in chickens:
        chicken_x, chicken_y = chicken 
        
        distance = abs(home_x - chicken_x) + abs(home_y - chicken_y)
        
        min_distance = min(min_distance, distance)
    
    return min_distance

min_distance = int(1e9)

for selected_chickens in combinations(chickens, M):
    sum_distance = 0
        
    for home in homes:
        distance = calculate_distance(home, selected_chickens)
            
        sum_distance += distance
            
    min_distance = min(min_distance, sum_distance)
    
print(min_distance)
            