import sys

input = sys.stdin.readline

board = []
for i in range(9):
    board.append(list(map(int, input().strip())))

row = [set() for _ in range(9)]
col = [set() for _ in range(9)]
box = [set() for _ in range(9)]

blanks = []

for i in range(9):
    for j in range(9):
        if board[i][j] == 0:
            blanks.append((i, j))
        else:
            n = board[i][j]
            row[i].add(n)
            col[j].add(n)
            box[(i // 3) * 3 + (j // 3)].add(n)

def dfs(idx):
    if idx == len(blanks):
        for line in board:
            print(''.join(map(str, line)))
        sys.exit()
    
    i, j = blanks[idx]
    b = (i // 3) * 3 + (j // 3)
    
    for n in range(1, 10):
        if n not in row[i] and n not in col[j] and n not in box[b]:
            board[i][j] = n
            row[i].add(n)
            col[j].add(n)
            box[b].add(n)
            
            dfs(idx + 1)
            
            board[i][j] = 0
            row[i].discard(n)
            col[j].discard(n)
            box[b].discard(n)

dfs(0)