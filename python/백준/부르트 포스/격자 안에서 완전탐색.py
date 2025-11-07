n = int(input())
grid = [list(map(int, input().split())) for _ in range(n)]
# ↘ 대각선 프리픽스: pref1[r+1][c+1] = grid[r][c] + pref1[r][c]
pref1 = [[0]*(n+1) for _ in range(n+1)]
for r in range(n):
    for c in range(n):
        pref1[r+1][c+1] = grid[r][c] + pref1[r][c]

# ↙ 대각선 프리픽스: pref2[r+1][c] = grid[r][c] + pref2[r][c+1]
pref2 = [[0]*(n+1) for _ in range(n+1)]
for r in range(n):
    for c in range(n-1, -1, -1):
        pref2[r+1][c] = grid[r][c] + pref2[r][c+1]

def sum_main(r1, c1, r2, c2):
    """↘ 대각선 구간합 (r1<=r2, c1<=c2, r2-r1 == c2-c1) [양 끝점 포함]"""
    return pref1[r2+1][c2+1] - pref1[r1][c1]

def sum_anti(r1, c1, r2, c2):
    """↙ 대각선 구간합 (r1<=r2, c1>=c2, r2-r1 == c1-c2) [양 끝점 포함]"""
    return pref2[r2+1][c2] - pref2[r1][c1+1]

def inb(r, c):
    return 0 <= r < n and 0 <= c < n

max_sum = -10**18
best = None  # (top_r, top_c, a, b)

# 마름모 정의:
# top=(r,c)
# right=(r+a, c+a)
# bottom=(r+a+b, c+a-b)
# left=(r+b, c-b)
# 각 변 길이는 a>=1, b>=1 (둘레가 생겨야 하므로)
for r in range(n):
    for c in range(n):
        a = 1
        while inb(r+a, c+a):  # right 꼭짓점이 범위 내일 때까지 확장
            b = 1
            while True:
                rr, rc = r + a, c + a           # right
                br, bc = r + a + b, c + a - b       # bottom
                lr, lc = r + b, c - b           # left

                if not (inb(lr, lc) and inb(br, bc)):
                    break  # b를 더 키울 수 없음

                # 네 변의 합(각 변은 양 끝점 포함)
                # top -> right (↘)
                s1 = sum_main(r, c, rr, rc)
                # right -> bottom (↙)
                s2 = sum_anti(rr, rc, br, bc)
                # left -> bottom (↘)
                s3 = sum_main(lr, lc, br, bc)
                # top -> left (↙)
                s4 = sum_anti(r, c, lr, lc)

                # 각 꼭짓점은 두 변에서 한 번씩 포함되므로 총 2번 카운트됨.
                # 우리가 원하는 건 둘레에서 각 꼭짓점을 한 번만 세는 것이므로
                # 꼭짓점 4개 값을 한 번씩 빼서 중복을 제거한다.
                perimeter = s1 + s2 + s3 + s4 \
                            - (grid[r][c] + grid[rr][rc] + grid[br][bc] + grid[lr][lc])

                if perimeter > max_sum:
                    max_sum = perimeter
                    best = (r, c, a, b)

                b += 1
            a += 1

print(max_sum)
# 필요하면 최적 형태도 출력
# if best:
#     tr, tc, a, b = best
#     print("top=({},{}), a={}, b={}".format(tr, tc, a, b))