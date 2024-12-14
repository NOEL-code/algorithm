a, b, c, d, e, f = map(int, input().split())

x = 0
y = 0

for i in range(-999, 1000, 1):
  for j in range(-1000, 1000, 1):
    if (a * i + b * j) == c and (d * i + e * j) == f:
      x = i
      y = j
      print(x, y)   
      break



a, b, c, d, e, f = map(int, input().split())

# 행렬식(Determinant) 계산
det = a * e - b * d

if det == 0:
    print("No unique solution")  # 유일한 해가 없을 경우
else:
    # Cramer's Rule 적용
    x = (c * e - b * f) // det
    y = (a * f - c * d) // det
    print(x, y)