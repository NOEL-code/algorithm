array = []
for _ in range(9):
    array.append(int(input()))
array.sort()
for i in range(len(array)):
    for j in range(i + 1, len(array)):
        if sum(array) - (array[i] + array[j]) == 100:
            for k in range(len(array)):
                if k == i or k == j:
                    pass
                else:
                    print(array[k])
            exit()
    
     
        
    