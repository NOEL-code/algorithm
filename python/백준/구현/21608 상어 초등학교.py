import sys

input = sys.stdin.readline
 
N = int(input())

like_dict = {}
order = []

for _ in range(N * N):
    data = list(map(int, input().split()))
    student = data[0]
    likes = data[1:]
    like_dict[student] = likes
    order.append(student)

seat = [[0] * N for _ in range(N)]

directions = [(0,1), (1,0), (-1,0), (0,-1)]  # 우, 하, 상, 좌 (순서는 상관 X)

def find_best_seat(student):
    likes = like_dict[student]
    best = None  # (like_cnt, empty_cnt, r, c)
    
    for r in range(N):
        for c in range(N):
            if seat[r][c] != 0:
                continue

            like_count = 0
            empty_count = 0

            for dr, dc in directions:
                nr, nc = r + dr, c + dc
                if 0 <= nr < N and 0 <= nc < N:
                    if seat[nr][nc] in likes:
                        like_count += 1
                    elif seat[nr][nc] == 0:
                        empty_count += 1
            
            if best is None:
                best = (like_count, empty_count, r, c)
            else:
                if like_count > best[0]:
                    best = (like_count, empty_count, r, c)
                elif like_count == best[0]:
                    if empty_count > best[1]:
                        best = (like_count, empty_count, r, c)
                    elif empty_count == best[1]:
                        if r < best[2] or (r == best[2] and c < best[3]):
                            best = (like_count, empty_count, r, c)

    return best[2], best[3]


for student in order:
    r, c = find_best_seat(student)
    seat[r][c] = student

score_table = [0, 1, 10, 100, 1000]
answer = 0

for r in range(N):
    for c in range(N):
        student = seat[r][c]
        likes = like_dict[student]
        like_cnt = 0
        
        for dr, dc in directions:
            nr, nc = r + dr, c + dc
            if 0 <= nr < N and 0 <= nc < N:
                if seat[nr][nc] in likes:
                    like_cnt += 1

        answer += score_table[like_cnt]

print(answer)