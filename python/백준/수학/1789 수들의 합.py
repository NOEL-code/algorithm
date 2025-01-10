s = int(input())

i = 0
sum = 0
while s > sum:
    i += 1
    sum += i

if sum == s:
    print(i)
else:
    print(i - 1)