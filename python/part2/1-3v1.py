position = input()

y = int(position[1])
x = int(ord(position[0])) - 97 + 1

count = 0

print(x, y)
if x + 2 <= 8 | y + 1 <= 8:
    count += 1

if x + 2 <= 8 | y - 1 >= 1:
    count += 1

if x - 2 >= 1 | y + 1 <= 8:
    count += 1

if x - 2 >= 1 | y - 1 >= 1:
    count += 1
    
    
if y + 2 <= 8 | x + 1 <= 8:
    count += 1

if y + 2 <= 8 | x - 1 >= 1:
    count += 1

if y - 2 >= 1 | x + 1 <= 8:
    count += 1

if y - 2 >= 1 | x - 1 >= 1:
    count += 1

print(count)