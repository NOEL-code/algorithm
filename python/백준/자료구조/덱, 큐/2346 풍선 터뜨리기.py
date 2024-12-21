from collections import deque

n = int(input())

dequeue = deque(enumerate(map(int, input().split())))

result = []

while dequeue:
  index, paper = dequeue.popleft()
  
  result.append(index + 1)
  if paper > 0:
    dequeue.rotate(-(paper - 1))
  elif paper < 0:
    dequeue.rotate(-paper)
    
print(" ".join(map(str, result)))