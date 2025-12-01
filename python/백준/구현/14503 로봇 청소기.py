import sys

input = sys.stdin.readline

N, M = map(int, input().split())
r, c, d = list(map(int, input().split()))
room = [list(map(int, input().split())) for _ in range(N)]
directions = [(-1, 0), (0, 1), (1, 0), (0, -1)]

def left(d):
    return (d + 3) % 4

count = 0

while True:
    if room[r][c] == 0:
        room[r][c] = 2
        count += 1
        
    moved = False

    for _ in range(4):
        d = left(d)
        dr, dc = directions[d]
        nr, nc = r + dr, c + dc
        if 0 <= nc < M and 0 <= nr < N and room[nr][nc] == 0:
            r, c = nr, nc
            moved = True
            break
            
    if moved:
        continue

    back_dir = (d + 2) % 4
    br, bc = directions[back_dir]
    nr, nc = r + br, c + bc

    if 0 <= nr < N and 0 <= nc < M and room[nr][nc] != 1:
        r, c = nr, nc
    else:
        print(count)
        break