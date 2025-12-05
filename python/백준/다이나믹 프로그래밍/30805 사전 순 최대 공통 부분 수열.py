import sys
input = sys.stdin.readline

N = int(input())
A = list(map(int, input().split()))
M = int(input())
B = list(map(int, input().split()))

# 현재 A, B에서 어디까지 썼는지 가리키는 포인터
ia = 0
ib = 0
ans = []

# 숫자 범위가 1~100
MAX_VAL = 100

while ia < N and ib < M:
    chosen = None
    next_ia = next_ib = None

    # 이번에 뽑을 값 v를 큰 것부터 내려가며 탐색
    for v in range(MAX_VAL, 0, -1):
        # A에서 v가 ia 이후에 처음 나오는 위치 찾기
        pa = -1
        for i in range(ia, N):
            if A[i] == v:
                pa = i
                break
        if pa == -1:
            continue  # A에는 더 이상 v가 없음

        # B에서도 v가 ib 이후에 처음 나오는 위치 찾기
        pb = -1
        for j in range(ib, M):
            if B[j] == v:
                pb = j
                break
        if pb == -1:
            continue  # B에는 더 이상 v가 없음

        # A, B 둘 다에서 ia/ib 이후에 v를 찾았으니, 이 v를 선택
        chosen = v
        next_ia = pa + 1
        next_ib = pb + 1
        break  # 가장 큰 v를 찾았으니 더 내려갈 필요 없음

    if chosen is None:  # 더 이상 공통으로 뽑을 수 있는 수가 없음
        break

    ans.append(chosen)
    ia = next_ia
    ib = next_ib

print(len(ans))
if ans:
    print(*ans)