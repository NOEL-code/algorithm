n, m = map(int, input().split())

array = list(map(int, input().split()))

def binary_search_recursive(array, target, start, end):
  
  if start > end:
    return end 
  mid = (start + end) // 2
  
  sum = 0
  
  for i in range(n):
    if array[i] - mid > 0:
      sum += (array[i] - mid) 
    
  if target == sum:
    return mid
  
  if target > sum:
    return binary_search_recursive(array, target, start, mid - 1)
  
  if target < sum:
    return binary_search_recursive(array, target, mid + 1, end)
  

array.sort()

print(binary_search_recursive(array, m, 0, array[n-1]))