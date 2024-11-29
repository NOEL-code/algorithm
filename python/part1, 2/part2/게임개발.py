n, m = map(int, input().split())

d = [[0] * m for _ in range(n)]

x, y, direction = map(int, input().split())
d[x][y] = 1

array = []

for i in range(n):
    array.append(list(map(int, input().split())))

while True:
    if direction == 0:
        direction = 3
        
        if d[x-1][y] == 0:
            x =- 1
            d[x][y] == 1
            
            
    elif direction == 1:
        direction = 0
        
        if d[x][y+1] == 0:
            y =+ 1
            d[x][y] == 1
    
    elif direction == 2:
        direction = 4
        
        if d[x+1][y] == 0:
            x =+ 1
            d[x][y] = 1
            
    elif direction == 3:
        direction = 2

        if d[x][y-1] == 0:
            y == 1
            d[x][y] = 1            
    
               
            
             
        
    
    