isbn = input()

check = 0

star_1 = False
star_3 = False

check = 0
for i in range(1, 13):
    if isbn[i] == '*':
        if i % 2 == 0:
            star_3 = True
        else:
            star_1 = True
    else:
        if i % 2 == 0:
            check += int(isbn[i]) * 3
        else:
            check += int(isbn[i])

for i in range(1, 10):
    if star_3:
        check += i * 3
    else:
        check += i

    r = check % 10

    if (10 - r) % 10 == isbn[12]:
        print(i)
        exit(0)