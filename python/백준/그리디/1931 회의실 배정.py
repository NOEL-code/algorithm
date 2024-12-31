import sys
input = sys.stdin.readline

N = int(input())

data = []

for _ in range(N):
    x, y = map(int, input().split()) 
    data.append((x,y))
  
data.sort(key=lambda x: (x[1], x[0]))

count = 1

end = data[0][1]

for i in range(1, N):
    if data[i][0] >= end:
        end = data[i][1]
        count += 1

print(count) 