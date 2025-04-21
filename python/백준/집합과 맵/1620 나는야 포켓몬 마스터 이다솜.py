import sys

input = sys.stdin.readline

N, M = map(int, input().split())


pocketMon_name = {}
pocketMon_int = {}
for i in range(1, N + 1):
    name = input().rstrip()
    pocketMon_name[name] = i
    pocketMon_int[i] = name

for i in range(M):
    quiry = input().rstrip()
    
    if quiry.isdigit():
        print(pocketMon_int[int(quiry)])
    else:
        print(pocketMon_name[quiry])
        
        
        