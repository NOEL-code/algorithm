N, M, x, y, K = map(int, input().split())
board = [list(map(int, input().split())) for _ in range(N)]
commands = list(map(int, input().split()))

dice = [0,0,0,0,0,0]

dx = [0, 0, 0, -1, 1]
dy = [0, 1, -1, 0, 0]

def roll(dice, direction):
    top, bottom, north, south, west, east = dice
    
    if direction == 1:  # 동
        return [west, east, north, south, bottom, top]
    elif direction == 2:  # 서
        return [east, west, north, south, top, bottom]
    elif direction == 3:  # 북
        return [south, north, top, bottom, west, east]
    elif direction == 4:  # 남
        return [north, south, bottom, top, west, east]

for cmd in commands:
    nx = x + dx[cmd]
    ny = y + dy[cmd]
    
    if not (0 <= nx < N and 0 <= ny < M):
        continue
    
    x, y = nx, ny
    
    dice = roll(dice, cmd)
    
    if board[x][y] == 0:
        board[x][y] = dice[1]
    else:
        dice[1] = board[x][y]
        board[x][y] = 0
    
    print(dice[0])