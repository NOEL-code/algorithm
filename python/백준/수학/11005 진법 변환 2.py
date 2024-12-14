n, b = map(int, input().split())

array = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"

result = ''

while(n > 0):
  result += str(array[n % b])
  
  n = n // b

print(result[::-1])