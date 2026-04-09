import sys

input = sys.stdin.readline

N, M, K = map(int, input().split())

directions = [(-1, 0), (-1, 1), (0, 1), (1, 1), (1, 0), (1, -1), (0, -1), (-1, -1)]

fireballs = []

for i in range(M):
    # r, c, m(질량), d(directions), s(speed)
    r, c, m, s, d = map(int, input().split())
    fireballs.append((r-1, c-1, m, s, d))

for _ in range(K):
    grid = [[[] for _ in range(N)] for _ in range(N)]
    for r, c, m, s, d in fireballs:
            dr, dc = directions[d]
            nr = (r + dr * s) % N
            nc = (c + dc * s) % N
            grid[nr][nc].append((m, s, d))

    fireballs = []
    for r in range(N):
        for c in range(N):
            if len(grid[r][c]) == 1:
                m, s, d = grid[r][c][0]
                fireballs.append((r,c,m,s,d))

            elif len(grid[r][c]) >= 2:
                sum_m, sum_s = 0, 0
                
                odd = 0
                even = 0
                
                for m, s, d in grid[r][c]:
                    if d % 2 == 0:
                        even += 1
                    else:
                        odd += 1
                    sum_m += m
                    sum_s += s
                
                even_flag = False
                
                if odd == 0 or even == 0:
                    even_flag = True
                
                new_s = sum_s // len(grid[r][c])
                new_m = sum_m // 5
                
                if new_m == 0:
                    continue
                
                if even_flag:
                    for i in range(0, 8, 2):
                        fireballs.append((r, c, new_m, new_s, i))

                else:
                    for i in range(1, 8, 2):
                        fireballs.append((r, c, new_m, new_s, i))

print(sum(m for r, c, m, s, d in fireballs))
        
        