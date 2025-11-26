import sys
input = sys.stdin.readline

N, K = map(int, input().split())
arr = list(map(int, input().split()))

cnt = [0] * 100001   # ai 범위: 1 ~ 100000
left = 0
right = 0
answer = 0

while right < N:
    # 현재 right 위치의 숫자를 더 넣어도 되는지 확인
    if cnt[arr[right]] < K:
        cnt[arr[right]] += 1
        right += 1
        # [left, right) 구간 길이 갱신
        answer = max(answer, right - left)
    else:
        # 이미 K개라 더 못 넣음 → left를 앞으로 당기면서 개수 줄이기
        cnt[arr[left]] -= 1
        left += 1

print(answer)