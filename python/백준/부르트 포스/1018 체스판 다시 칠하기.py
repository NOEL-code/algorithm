n,m = map(int, input().split())

board = []
result = []

for _ in range(n):
  board.append(input())

for i in range(n - 7):
  for j in range(m - 7):
    draw_white = 0
    draw_black = 0
    
    for a in range(i, i + 8):
      for b in range(j, j + 8):
          if (a + b) % 2 == 0:
            if board[a][b] != "W":
              draw_white += 1
            else:
              draw_black += 1
          else:
            if board[a][b] != "W":
              draw_black += 1
            else: 
              draw_white += 1
    
    result.append(min(draw_black, draw_white))
          
print(min(result))