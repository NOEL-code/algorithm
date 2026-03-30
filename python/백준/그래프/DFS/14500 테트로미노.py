import sys
input = sys.stdin.readline

N, M = map(int, input().split())
board = [list(map(int, input().split())) for _ in range(N)]

visited = [[False] * M for _ in range(N)]

directions = [(0, 1), (1, 0), (0, -1), (-1, 0)]
ans = 0

def dfs(x, y, depth, total):
    global ans
    neighbors = []
    for dx, dy in directions:
        nx, ny = x + dx, y + dy
        if 0 <= nx < N and 0 <= ny < M and not visited:
            visited[nx][ny] = True
            dfs(nx, ny, depth+ 1, total + board[nx][ny])
            visited[nx][ny] = False
    
def check_t(x, y):    
    global ans
    neighbors = []
    for dx, dy in directions:
        nx, ny = x + dx, y + dy
        if 0 <= nx < N and 0 <= ny < M and not visited:
            neighbors.append(board[nx][ny])

    if len(neighbors) >= 3:
        if len(neighbors) == 4:
            ans = max(ans, board[x][y] + sum(neighbors) - min(neighbors))
        else:
            ans = max(ans, board[x][y] + sum(neighbors))

for i in range(N):
    for j in range(M):
        visited[i][j] = True
        dfs(i, j, 1, board[i][j])
        visited[i][j] = False
        check_t(i, j)

print(ans)