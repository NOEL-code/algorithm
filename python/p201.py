n, m = list(map(int, input().split(' ')))
array = list(map(int, input().split()))

array.sort()
def binary_search(array, target, start, end):
  
  result = 0
  while(start <= end):
    mid = (start + end) // 2 
    sum = 0
    
    for i in array:
      if i > mid:
        sum += (i - mid)
  
    if sum == target:
      return mid
    elif sum > target:
      start = mid + 1
      result = mid
    else:
      end = mid - 1 
  
  return result

result = binary_search(array, m, 0, max(array))


print(result)
