def back_tracking(arr, now_length, current_num):
    if now_length == M:
        print(*arr)
        return

    for idx in range(current_num, N + 1):
        arr.append(idx)
        back_tracking(arr, now_length, idx)
        arr.pop()


N, M = map(int, input().split())

num = [i + 1 for i in range(N + 1)]

back_tracking([], 0, 1)