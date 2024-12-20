import sys
import heapq

input = sys.stdin.readline

heap = []
n = int(input())

for _ in range(n):
    x = int(input())
    if x != 0:
        # 절댓값을 기준으로 힙에 삽입
        heapq.heappush(heap, (abs(x), x))
    else:
        if not heap:
            print(0)
        else:
            # 절댓값이 가장 작은 값 반환
            print(heapq.heappop(heap)[1])