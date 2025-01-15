def back_tracking(arr, now_length, start, num_arr):
    if now_length == M:
        print(*arr)  # 조건을 만족하는 수열 출력
        return

    for idx in range(start, len(num_arr)):  # 현재 인덱스부터 탐색
        arr.append(num_arr[idx])  # 숫자 추가
        back_tracking(arr, now_length + 1, idx, num_arr)  # 재귀 호출
        arr.pop()  # 백트래킹: 숫자 제거

# 입력 처리
N, M = map(int, input().split())
num_arr = sorted(set(map(int, input().split())))  # 중복 제거 후 정렬

# 백트래킹 호출
back_tracking([], 0, 0, num_arr)