def merge_sort(arr, tmp, left, right):
    global count, result
    if left < right:
        mid = (left + right) // 2
        merge_sort(arr, tmp, left, mid)
        merge_sort(arr, tmp, mid + 1, right)
        merge(arr, tmp, left, mid, right)

def merge(arr, tmp, left, mid, right):
    global count, result

    i = left          # 왼쪽 부분 배열의 시작
    j = mid + 1       # 오른쪽 부분 배열의 시작
    t = left          # 임시 배열 `tmp`의 시작

    # 병합 과정
    while i <= mid and j <= right:
        if arr[i] <= arr[j]:
            tmp[t] = arr[i]
            i += 1
        else:
            tmp[t] = arr[j]
            j += 1
        count += 1
        if count == K:  # K번째 저장 시 결과값 저장
            result = tmp[t]
        t += 1

    # 왼쪽 배열의 나머지
    while i <= mid:
        tmp[t] = arr[i]
        i += 1
        count += 1
        if count == K:
            result = tmp[t]
        t += 1

    # 오른쪽 배열의 나머지
    while j <= right:
        tmp[t] = arr[j]
        j += 1
        count += 1
        if count == K:
            result = tmp[t]
        t += 1

    # 정렬된 결과를 원본 배열로 복사
    for t in range(left, right + 1):
        arr[t] = tmp[t]


# 입력 처리
N, K = map(int, input().split())
arr = list(map(int, input().split()))

# 초기화
tmp = [0] * N
count = 0
result = -1

# 병합 정렬 실행
merge_sort(arr, tmp, 0, N - 1)

# 결과 출력
print(result)