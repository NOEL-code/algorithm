import sys

input = sys.stdin.readline

N = int(input())

_dict = {}
count = 1
flag = False

for _ in range(N):
    a, b = map(str, input().split())

    if not flag and (a == "ChongChong" or b == "ChongChong"):
        flag = True
        _dict[a] = 1
        _dict[b] = 1
        count += 1
        
    if flag and a in _dict and b not in _dict:
        _dict[b] = 1
        count += 1
    
    if flag and  a not in _dict and b in _dict:
        _dict[a] = 1
        count += 1
        
print(count)
            
