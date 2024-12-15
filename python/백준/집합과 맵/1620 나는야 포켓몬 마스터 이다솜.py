import sys

input = sys.stdin.readline

n, m = map(int, input().split())


never_heard= {}
never_seen = {}
never_heard_and_seen = [] 
count = 0


for i in range(n):
  name = input().rstrip()
  never_heard[name] = 0

for i in range(m):
  name = input().rstrip()
  never_seen[name] = 0
  if name in never_heard:
    never_heard_and_seen.append(name)
    count += 1
    
print(count)
never_heard_and_seen.sort()

for name in never_heard_and_seen:
  print(name)
  



n, m = map(int, input().split())

never_heard = set()

for i in range(n):
  never_heard.add(input())

never_seen = set()

for i in range(m):
  never_seen.add(input())
  
never_heard_and_seen = sorted(list(never_heard & never_seen))

print(len(never_heard_and_seen))

for name in never_heard_and_seen:
  print(name)
