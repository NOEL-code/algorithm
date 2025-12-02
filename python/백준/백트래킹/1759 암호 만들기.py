import sys

input = sys.stdin.readline
 
L, C = map(int, input().split())

alhabet = list(input().split())
alhabet.sort()  # 사전 순을 위해 미리 정렬하는 게 안전함
used_alhabet = [False] * C

vowels = {'a', 'e', 'i', 'o', 'u'}
answer = []

def back_tracking(arr, now_length):
    if now_length == L:
        # 모음/자음 조건 체크
        vowel_cnt = sum(1 for ch in arr if ch in vowels)
        consonant_cnt = L - vowel_cnt
        if vowel_cnt >= 1 and consonant_cnt >= 2:
            answer.append(''.join(arr))
        return

    for idx in range(C):
        if not used_alhabet[idx]:
            # 사전 순(오름차순) 유지
            if now_length == 0 or arr[now_length-1] < alhabet[idx]:
                used_alhabet[idx] = True
                arr.append(alhabet[idx])
                back_tracking(arr, now_length+1)
                arr.pop()
                used_alhabet[idx] = False

back_tracking([], 0)

# answer 자체가 이미 사전 순이지만, 혹시 몰라 정렬 한 번 더
answer.sort()

for comb in answer:
    print(comb)