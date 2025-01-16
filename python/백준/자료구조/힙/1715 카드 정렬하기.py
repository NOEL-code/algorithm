import heapq
n = int(input())

cards = []
result = 0
for _ in range(n):
    heapq.heappush(cards, int(input()))

while len(cards) > 1:
    first = heapq.heappop(cards)
    second = heapq.heappop(cards)
    
    result += (first + second)
    heapq.heappush(cards, first + second)

print(result)