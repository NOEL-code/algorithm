import sys
input = sys.stdin.readline

N, r, c = map(int, input().split())

array_size = 2 ** N  # 최종 배열의 크기 계산

# 0으로 초기화된 2차원 배열 생성
array = [[0 for _ in range(array_size)] for _ in range(array_size)]

count = 0

def search(x, y, size):
  global count  
  if size > 2:
    new_size = size // 2
    search(x, y, new_size)
    search(x + new_size, y, new_size)
    search(x, y + new_size, new_size)
    search(x + new_size, y + new_size, new_size)
    return
  
  for i in range(x, x + size):
    for j in range(y, y + size):
      array[i][j] = count
      count += 1
  
  return
    
search(0, 0, array_size)
print(array[c][r])