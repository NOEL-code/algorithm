import sys
input = sys.stdin.readline

x = int(input())

line = 1
num = 1

while(x > num):
  num += line
  line += 1

if x != num:
  line -= 1
  num = num - line

i = x - num

if line % 2 == 0:
  mother = line
  child = 1
else:
  child = line
  mother = 1

for _ in range(i):
  if line % 2 == 0:
    mother -= 1
    child += 1
  else:
    mother += 1
    child -= 1

print(f'{child}/{mother}')
