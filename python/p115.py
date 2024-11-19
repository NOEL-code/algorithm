input_data = input()

row = int(input_data[1])
column = int(ord(input_data[0])) - int(ord('a')) + 1

dx = [2, 2, -2, -2, 1, -1, 1, -1]
dy = [1, -1, 1, -1, 2, 2, -2, -2]

result = 0

for i in range(8):
  if row + dx[i] >= 1 and row + dx[i] <= 8 and column + dy[i] >= 1 and column + dy[i] <= 8:
    result += 1

print(result)