def binary_search_recursive(array, target, start, end):
  if start > end:
    return None
  mid = (start + end) / 2
  
  if array[mid] == target:
    return mid
  elif array[mid] > target:
    return binary_search_recursive(array, start, mid -1)
  elif array[mid] < target:
    return binary_search_recursive(array, mid + 1, end)
  

def binary_search_for(array, target, start, end):
  while start <= end:
    mid = (start + end) // 2
    if array[mid] == target:
      return mid
    elif array[mid] > target:
      end = mid - 1
    else:
      start = mid + 1
  return None 