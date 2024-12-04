n, m = map(int, input().split())

result = 0
max_value = 0

for i in range(n):
  data = list(map(int, input().split()))
  
  min_in_data = min(data)
  
  max_value = max(data, max_value)
  
print(max_value)