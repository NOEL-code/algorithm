import sys
input = sys.stdin.readline

n, m = map(int, input().rstrip().split())

s = set()
count = 0

for _ in range(n):
    s.add(input().rstrip())
  
for _ in range(m):
    if input().rstrip() in s:
        count += 1
    
print(count)
  
  