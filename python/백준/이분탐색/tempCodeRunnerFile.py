X, Y = map(int, input().split())
Z = int((Y / X) * 100)

def search():
    start = 1
    end = X
    min_value = int(1e9)
    
    while start <= end:        
        mid = (start + end) // 2
        percent = int((Y + mid) / (X + mid) * 100)
        if percent > Z:
            end = mid - 1     
            min_value = mid  
        else:
            start = mid + 1
    return min_value

if X == Y:
    print(-1)
else:
    print(search())        