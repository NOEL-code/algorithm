import sys
import heapq

input = sys.stdin.readline
lectures = []

N = int(input())

for _ in range(N):
    start, end = map(int, input().split())
    heapq.heappush(lectures, (start, end))

end = heapq.heappop(lectures)[1]

for i in range(1, N):
    next_start, next_end = heapq.heappop(lectures)
    if next_start >= end:
        end = next_end
    else:
        heapq.heappush(lectures, (next_start, next_end))

print(len(lectures))