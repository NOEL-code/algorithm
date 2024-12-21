import sys
import heapq
input = sys.stdin.readline

n = int(input())

heap = []

for _ in range(n):
  num = int(input())
  
  if num != 0:
    heapq.heappush(heap, -num)
  else:
    if heap:
      print(-(heapq.heappop(heap)))
    else:
      print(0)  