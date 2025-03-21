import sys
import heapq

input = sys.stdin.readline
lectures = []

N = int(input().strip())

for _ in range(N):
    start, end = map(int, input().split())
    lectures.append((start, end))
lectures.sort()

heap = []
heapq.heappush(heap, lectures[0][1])

for i in range(1, N):
    start, end = lectures[i]

    if heap[0] <= start:
        heapq.heappop(heap)
    heapq.heappush(heap, end)

print(len(heap))