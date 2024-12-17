import sys
input = sys.stdin.readline

N, M = map(int, input().split())
trees = list(map(int, input().split()))

start = 0
end = max(trees)

result = 0

while start <= end:
  mid = (start + end) // 2
  log = 0
  
  for i in trees:
    if i >= mid:
      log += i - mid
    
  if log < M:
    end = mid - 1
  else:
    result = mid
    start = mid + 1
          
print(result)