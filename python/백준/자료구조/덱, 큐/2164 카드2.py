from collections import deque

cards = deque()

n = int(input())

for i in range(1, n + 1):
  cards.append(i)
  
while not len(cards) == 1:
  cards.popleft()
  cards.append(cards.popleft())

print(cards.popleft())