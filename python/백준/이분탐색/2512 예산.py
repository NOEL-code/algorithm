import sys
input = sys.stdin.readline

def search(start, end):
    result = 0
    
    while start <= end:
        mid = (start + end) // 2
        total = sum(min(mid, num) for num in data)
        
        if total <= limit:
            result = mid
            start = mid + 1
        else:
            end = mid - 1
            
    return result

N = int(input())
data = list(map(int, input().split()))
limit = int(input())
max_val = max(data)

if sum(data) <= limit:
    print(max_val)
else:
    print(search(0, max_val))