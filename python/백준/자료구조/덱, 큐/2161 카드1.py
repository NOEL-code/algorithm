from collections import deque

n = int(input())
cards = deque()

for i in range(1, n+1):
    cards.append(i)

while cards:
    if cards:
        print(cards.popleft(), end=" ")
    if cards:
        cards.append(cards.popleft())
