import sys
input = sys.stdin.readline

M = int(input())

_set = set()

for _ in range(M):
    command = input().split()

    if command[0] == 'add':
        _set.add(int(command[1]))  # 정수로 변환하여 추가

    elif command[0] == 'remove':
        if int(command[1]) in _set:  # 정수로 변환 후 확인
            _set.remove(int(command[1]))

    elif command[0] == 'check':
        if int(command[1]) in _set:
            print(1)
        else:
            print(0)

    elif command[0] == 'toggle':
        if int(command[1]) in _set:
            _set.remove(int(command[1]))
        else:
            _set.add(int(command[1]))

    elif command[0] == 'all':
        _set = set(range(1, 21))  # 1부터 20까지의 집합 생성

    elif command[0] == 'empty':
        _set = set()  # 빈 집합 생성