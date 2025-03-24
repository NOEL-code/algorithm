def back_tracking(arr, now_length):
    if now_length == M:
        print(*arr)
        return

    for idx in range(1, N + 1):
        arr.append(idx)
        back_tracking(arr, now_length + 1)
        arr.pop()


N, M = map(int, input().split())

num = [i + 1 for i in range(N + 1)]

back_tracking([], 0)