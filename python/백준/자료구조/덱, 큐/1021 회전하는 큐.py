import sys
from collections import deque
input = sys.stdin.readline

N, M = map(int, input().split())

numbers = list(map(int, input().split()))
dequeue = deque([i for i in range(1, N + 1)])

count = 0
while(numbers):
    if dequeue[0] == numbers[0]:
        dequeue.popleft()
        numbers.pop(0)
    else:
        if dequeue.index(numbers[0]) <= len(dequeue) // 2:
            dequeue.rotate(-1)
            count += 1
        else:
            dequeue.rotate(1)
            count += 1
        
print(count)