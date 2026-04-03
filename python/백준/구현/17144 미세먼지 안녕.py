import sys

input = sys.stdin.readline

R, C, T = map(int, input().split())

board = [list(map(int, input().split())) for _ in range(R)]

directions = [(1, 0), (0, 1), (-1, 0), (0, -1)]

def diffuse(board,temp_board):
    for r in range(R):
        for c in range(C):
            cnt = 0
            if board[r][c] > 0:
                for dx, dy in directions:
                    nx, ny = r + dx, c + dy
                    if 0 <= nx < R and 0 <= ny < C and board[nx][ny] > -1:
                        temp_board[nx][ny] +=  board[r][c] // 5
                        cnt += 1
                temp_board[r][c] -= (board[r][c] // 5) * cnt
    
    for r in range(R):
        for c in range(C):
            board[r][c] += temp_board[r][c]
 
def air_cleaner(board, top):
    # 위쪽 순환 (반시계): top행 기준
    # ① 왼쪽 열 아래로
    for i in range(top - 1, 0, -1):
        board[i][0] = board[i - 1][0]
    # ② 윗행 오른쪽으로
    for i in range(C - 1):
        board[0][i] = board[0][i + 1]
    # ③ 오른쪽 열 위로
    for i in range(top):
        board[i][C - 1] = board[i + 1][C - 1]
    # ④ top행 왼쪽으로
    for i in range(C - 1, 1, -1):
        board[top][i] = board[top][i - 1]
    board[top][1] = 0  # 공기청정기에서 나온 바람

    # 아래쪽 순환 (시계): top+1행 기준
    bot = top + 1
    for i in range(bot + 1, R - 1):
        board[i][0] = board[i + 1][0]
    for i in range(C - 1):
        board[R - 1][i] = board[R - 1][i + 1]
    for i in range(R - 1, bot, -1):
        board[i][C - 1] = board[i - 1][C - 1]
    for i in range(C - 1, 1, -1):
        board[bot][i] = board[bot][i - 1]
    board[bot][1] = 0


top = 0

for i in range(R):
    if board[i][0] == -1:
        top = i
        break
           
for _ in range(T):
    temp_board = [[0] * C for _ in range(R)]
    
    diffuse(board, temp_board)
    air_cleaner(board, top)
    
answer = 0

for i in range(R):
    answer += sum(board[i]) 
    
print(answer + 2)