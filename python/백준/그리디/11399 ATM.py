N = int(input())

queue = list(map(int, input().split()))

queue.sort()


result = []

for i in range(N):
    result.append(queue[i])
    if i < N - 1:  # 마지막 인덱스가 아닌 경우에만 수행
        queue[i + 1] += queue[i]
print(sum(result))