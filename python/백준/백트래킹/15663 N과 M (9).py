def back_tracking(arr, now_length, num_arr, used_index_arr, result):
    if now_length == M:
        result.append(arr[:])  # 결과 리스트에 현재 배열 복사본 저장
        return

    prev = -1  # 이전에 선택한 숫자를 추적
    for idx in range(len(num_arr)):
        if not used_index_arr[idx] and num_arr[idx] != prev:
            used_index_arr[idx] = True
            arr.append(num_arr[idx])
            back_tracking(arr, now_length + 1, num_arr, used_index_arr, result)
            used_index_arr[idx] = False
            arr.pop()
            prev = num_arr[idx]  # 현재 숫자를 이전 값으로 업데이트

N, M = map(int, input().split())
num_arr = list(map(int, input().split()))
used_index_arr = [False] * len(num_arr)
result = []

back_tracking([], 0, sorted(num_arr), used_index_arr, result)

for r in result:
    print(" ".join(map(str, r)))