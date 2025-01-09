import sys
input = sys.stdin.readline

array = input().strip().split('-')

result = 0

for num in array[0].split('+'):
    result += int(num)
    
temp = []
for string in array[1:]:
    temp_sum = 0
    for num in string.split('+'):
        temp_sum += int(num)
    temp.append(temp_sum)

result -= sum(temp)

print(result)