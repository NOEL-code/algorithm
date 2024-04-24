n = int(input())

x, y = 1, 1

commands = input().split()

for command in commands:
    if command == "R":
        if x == n :
            continue
        x += 1
    elif command == "L":
        if x == 1:
            continue
        x -= 1
    elif command == "U":
        if y == 1:
            continue
        y -= 1
    else:
        if y == n:
            continue
        y += 1

print(y, x)