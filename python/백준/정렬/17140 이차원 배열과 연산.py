# R 연산 행이 열보다 많을 때, 연산 후에는 가장 큰 행을 기준으로 모든 행의 크기가 변화, 나머지는 0
# C 연산 열이 행보다 많을 때, 연산 후에는 가장 큰 열을 기준으로 모든 열의 크기가 변화, 나머지는 0

# 각 수가 몇 번 나왔는지 확인

# 수의 등장 횟수가 커지는 순으로, 같을 때는 숫자가 큰 순으로
# 0은 무시하기
# 다시 배열 A에 정렬된 결과 넣기
# 숫자와 등장 횟수 함께, 숫자가 먼저
import sys

input = sys.stdin.readline

R, C, K = map(int, input().split())

A = [[0] * 100 for _ in range(100)]

row_count = 3
col_count = 3

for r in range(3):
    numbers = list(map(int, input().split()))
    for c in range(3):
        A[r][c] = numbers[c]
        
for t in range(101):
    if R <= row_count and C <= col_count and A[R-1][C-1] == K:
        print(t)
        sys.exit()
        
    if t == 100:
        break

    if row_count >= col_count:
        new_col = 0
        for r in range(row_count):
            count = [0] * 101
            for c in range(col_count):
                if A[r][c] != 0:
                    count[A[r][c]] += 1

            pairs = []
            for num in range(1, 101):
                if count[num] > 0:
                    pairs.append((count[num], num))
            pairs.sort()

            A[r] = [0] * 100  # 이 행을 초기화

            idx = 0
            for freq, num in pairs:
                if idx >= 100:
                    break
                A[r][idx] = num
                A[r][idx + 1] = freq
                idx += 2
            new_col = max(new_col, idx)
        col_count = min(new_col, 100)
    
    else:
        new_row = 0
        for c in range(col_count):
            count = [0] * 101
            for r in range(row_count):
                if A[r][c] != 0:
                    count[A[r][c]] += 1

            pairs = []
            for num in range(1, 101):
                if count[num] > 0:
                    pairs.append((count[num], num))
            pairs.sort()
            
            for r in range(100):
                A[r][c] = 0
                
            idx = 0
            
            for freq, num in pairs:
                if idx >= 100:
                    break
                A[idx][c] = num
                A[idx + 1][c] = freq
                idx += 2
            new_row = max(new_row, idx)
        row_count = min(new_row, 100)

print(-1)