import sys

input = sys.stdin.readline

N = int(input())

paper = []
for _ in range(N):
  paper.append(list(map(int, input().split())))
  
white_count = 0
blue_count = 0

def divide(x, y, size):
  global white_count, blue_count
  color = paper[x][y]
  
  for i in range(x):
    for j in range(y):
      if paper[i][j] != color:
        new_size = size // 2
        
        divide(x, y, new_size)
        divide(x + new_size, y, new_size)
        divide(x, y + new_size, new_size)
        divide(x + new_size, y + new_size, new_size)
        return
      
  if color == 0:
    white_count += 1
  else:
    blue_count += 1
  
divide(0, 0, N)

print(white_count)
print(blue_count)
  