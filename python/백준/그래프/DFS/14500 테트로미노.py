import sys
input = sys.stdin.readline

N, M = map(int, input().split())
board = [list(map(int, input().split())) for _ in range(N)]

visited = [[False] * M for _ in range(N)]
ans = 0
dx = [0, 0, 1, -1]
dy = [1, -1, 0, 0]

# DFS로 I, O, S, Z, L 모양 탐색 (깊이 4)
def dfs(x, y, depth, total):
    global ans
    if depth == 4:
        ans = max(ans, total)
        return
    for i in range(4):
        nx, ny = x + dx[i], y + dy[i]
        if 0 <= nx < N and 0 <= ny < M and not visited[nx][ny]:
            visited[nx][ny] = True
            dfs(nx, ny, depth + 1, total + board[nx][ny])
            visited[nx][ny] = False

# T(ㅗ) 모양: 가운데 칸 + 상하좌우 중 3칸
def check_t(x, y):
    global ans
    neighbors = []
    for i in range(4):
        nx, ny = x + dx[i], y + dy[i]
        if 0 <= nx < N and 0 <= ny < M:
            neighbors.append(board[nx][ny])
    if len(neighbors) >= 3:
        # 4칸 다 있으면 가장 작은 것 하나를 빼고 3칸 선택
        # 3칸만 있으면 그대로
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