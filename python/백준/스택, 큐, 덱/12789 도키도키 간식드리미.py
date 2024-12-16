n = int(input())

student = list(map(int, input().split()))
stack = []
count = 1

for i in range(n):
  if student[i] == count:
    count += 1
  else:
    stack.append(student[i])
  
  while stack and stack[-1] == count:
      stack.pop()
      count += 1
      

if stack:
  print("Sad")
else:
  print("Nice")