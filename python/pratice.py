n = int(input())

array = []

for _ in range(n):
    array.append(input())

array = list(set(array)) # 중복제거
array.sort()

for i in array:
    print(i)