array = []

for i in range(5):
    array.append(int(input()))
    
array.sort()

print(array[2])
print(sum(array) // 5)