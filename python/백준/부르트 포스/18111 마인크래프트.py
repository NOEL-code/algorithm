import sys

input = sys.stdin.readline

N, M, B = map(int, input().split())

graph = [list(map(int, input().split())) for _ in range(N)]
time = (1e9)

for height in range(257):
    for x in range(N):
        for y in range(M):
            if graph[x][y] > height:
                taken_block = graph[x][y] - height                
            else:
                used_block = height - graph[x][y]                            
    
            if used_block > taken_block + B:
                continue
            
            temp_time = taken_block * 2 + used_block
            
            if temp_time <= time:
                time = temp_time
                glevel = height
            
print(time, glevel)