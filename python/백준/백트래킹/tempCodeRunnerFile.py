n = int(input())

count = 0
columns = [False] * n          # 세로 상태
diagonals1 = [False] * (2 * n) # 좌측 대각선 (x + y)
diagonals2 = [False] * (2 * n) # 우측 대각선 (x - y)

def n_queen(row):
    global count
    if row == n:  # 모든 퀸을 배치한 경우
        count += 1
        return
    
    for col in range(n):
        if not columns[col] and not diagonals1[row + col] and not diagonals2[row - col + n]:
            # 퀸을 배치
            columns[col] = diagonals1[row + col] = diagonals2[row - col + n] = True
            n_queen(row + 1)
            # 퀸을 제거 (백트래킹)
            columns[col] = diagonals1[row + col] = diagonals2[row - col + n] = False

n_queen(0)
print(count)