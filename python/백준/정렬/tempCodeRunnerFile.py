n = int(input())

array = list(map(int, input().split()))

array_s = sorted(set(array))

dic = {array_s[i]: i for i in range(len(array))}
      
  
for a in array:
  print(a, end=" ")