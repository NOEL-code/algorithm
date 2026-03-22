N, M = map(int, input().split())

books = list(map(int, input().split()))


left = []
right = []

for x in books:
    if x < 0:
        left.append(abs(x))
    else:
        right.append(x)

left.sort(reverse=True)
right.sort(reverse=True)

result = 0

for i in range(0, len(left), M):
    result += left[i] * 2

for i in range(0, len(right), M):
    result += right[i] * 2

# 🔥 가장 먼 거리 한 번 빼기
max_dist = 0
if left:
    max_dist = max(max_dist, left[0])
if right:
    max_dist = max(max_dist, right[0])

result -= max_dist

print(result)