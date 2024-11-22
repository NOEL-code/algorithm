array = [7, 5, 9, 0, 3, 1, 6, 2, 4, 8]

def selection_sort(array):
  for i in range(len(array)):
    min_index = i
    for j in range(i + 1, len(array)):
      if array[i] < array[min_index]:
        min_index = j
        array[min_index], array[i] = array[i], array[min_index]
        
  print(array)
  

def insertion_sort(array):
  for i in range(len(array)):
    for j in range(i, 0, -1):
      if array[j] < array[j - 1]:
         array[j], array[j - 1] = array[j - 1], array[j]
      else:
        break
  
  print(array)
      
def quick_sort(array, start, end):
  if start >= end:
    return
  pivot = start
  left = start + 1
  right = end
  while pivot <= right:
    while left <= end and array[left] <= array[pivot]:
      left += 1
      
    while right > start and array[right] >= array[pivot]:
      right -= 1
      
    if left > right:
      array[right], array[pivot] = array[pivot], array[right]
    else:
      array[left], array[right] = array[right], array[left]
      
  quick_sort(array, start, right - 1)
  quick_sort(array, right + 1, end)
  
  print(quick_sort(array, 0, len(array) - 1))
  
  