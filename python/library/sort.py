import heapq

array = [7, 5, 9, 0, 3, 1, 6, 2, 4, 8]

def selection_sort(array):
  for i in range(len(array)):
    min_index = i
    for j in range(i + 1, len(array)):
      if array[min_index] > array[j]:
        min_index = j
    array[i], array[min_index] = array[min_index], array[i]
        
  print(array)
  
def bubble_sort(array):
  for i in range(len(array)):
    for j in range(i + 1, len(array)):
      if array[i] > array[j]:
        array[i], array[j] = array[j], array[i]
          
def insertion_sort(array):
  for i in range(len(array)):
    for j in range(i, 0, -1):
      if array[j] < array[j - 1]:
         array[j], array[j - 1] = array[j - 1], array[j]
      else:
        break
  
  print(array)
      
def merge_sort(array):
  if len(array) <= 1:
    return array
  
  mid = len(array) // 2
  left = merge_sort(array[:mid])
  right = merge_sort(array[mid:])
  
  return merge(left, right)

def merge(left, right):
  result = []
  i = j = 0
  
  while i < len(left) and j < len(right):
    if left[i] <= right[j]:
      result.append(left[i])
      i += 1
    else:
      result.append(right[j])
      j += 1
  
  result.extend(left[i:])
  result.extend(right[j:])
    
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
  

def heapify(array, n, i):
    largest = i  # 현재 루트를 가장 큰 값으로 설정
    left = 2 * i + 1  # 왼쪽 자식
    right = 2 * i + 2  # 오른쪽 자식

    # 왼쪽 자식이 루트보다 크면 왼쪽 자식을 가장 큰 값으로 설정
    if left < n and array[left] > array[largest]:
        largest = left

    # 오른쪽 자식이 현재 가장 큰 값보다 크면 오른쪽 자식을 가장 큰 값으로 설정
    if right < n and array[right] > array[largest]:
        largest = right

    # 가장 큰 값이 루트가 아니라면 교환하고 재귀적으로 힙 속성을 확인
    if largest != i:
        array[i], array[largest] = array[largest], array[i]
        heapify(array, n, largest)

def heap_sort(array):
    n = len(array)

    # 힙 구성 (Build Max-Heap)
    for i in range(n // 2 - 1, -1, -1):
        heapify(array, n, i)

    # 힙 정렬
    for i in range(n - 1, 0, -1):
        array[i], array[0] = array[0], array[i]  # 현재 루트(최대값)를 배열 끝으로 이동
        heapify(array, i, 0)  # 남은 배열에 대해 힙 속성 유지
        
        
        
def heap_sort_heapq(array):
    heap = []
    sorted_array = []

    # 배열을 힙으로 변환
    for element in array:
        heapq.heappush(heap, element)

    # 힙에서 요소를 하나씩 꺼내 정렬된 배열에 추가
    while heap:
        sorted_array.append(heapq.heappop(heap))

    return sorted_array