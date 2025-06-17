import sys
input = sys.stdin.readline

N = int(input())
count = 0
_seen = set()  # set 사용이 더 간단하고 빠름

for _ in range(N):
    nickname = input().strip()
    
    if nickname == "ENTER":
        _seen.clear()  # set 초기화
    elif nickname not in _seen:
        _seen.add(nickname)
        count += 1

print(count)