import sys
sys.setrecursionlimit(10**6)
input = sys.stdin.readline

R, C = map(int, input().split())
board = [list(input().strip()) for _ in range(R)]

dx = [1, -1, 0, 0]
dy = [0, 0, 1, -1]

max_dist = 0

def dfs(x, y, bitmask, depth):
    global max_dist
    if depth > max_dist:
        max_dist = depth

    for i in range(4):
        nx = x + dx[i]
        ny = y + dy[i]

        if 0 <= nx < R and 0 <= ny < C:
            next_bit = 1 << (ord(board[nx][ny]) - ord('A'))

            # 아직 사용하지 않은 알파벳이면
            if not (bitmask & next_bit):
                dfs(nx, ny, bitmask | next_bit, depth + 1)

start_bit = 1 << (ord(board[0][0]) - ord('A'))
dfs(0, 0, start_bit, 1)

print(max_dist)