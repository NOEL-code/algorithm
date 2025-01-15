def back_tracking(arr, now_length, num_arr, used_index_arr):
    if now_length == M:
        print(*arr)
        return
    
    for idx in range(len(num_arr)):
        if not used_index_arr[idx]:
            arr.append(num_arr[idx])
            used_index_arr[idx] = True
            back_tracking(arr, now_length + 1, num_arr, used_index_arr)
            arr.pop()
            used_index_arr[idx] = False
            
N, M = map(int, input().split())

num_arr = list(map(int, input().split()))
used_index_arr = [False] * len(num_arr)
back_tracking([], 0, sorted(num_arr), used_index_arr)