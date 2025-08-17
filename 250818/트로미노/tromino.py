n, m = map(int, input().split())
grid = [list(map(int, input().split())) for _ in range(n)]

block = [[(0,0), (1,0), (1,1)], [(0,0), (0,1), (1,1)], [(0,0), (0,1), (1,0)], [(1,0), (0,1), (1,1)], [(0, 0), (0, 1), (0, 2)], [(0, 0), (1,0), (2,0)]]

count = 0

for i in range(n):
    for j in range(m):
        for a, b, c in block:
            x1, y1 = i + a[0], j + a[1]
            x2, y2 = i + b[0], j + b[1]
            x3, y3 = i + c[0], j + c[1]

            if x1 >= n or y1 >= m or x2 >= n or y2 >= m or x3 >= n or y3 >= m:
                continue

            count = max(count, grid[x1][y1] + grid[x2][y2] + grid[x3][y3])


print(count)


