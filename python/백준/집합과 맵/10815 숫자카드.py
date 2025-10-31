n = int(input())

cards = list(map(int, input().split(" ")))

m = int(input())

checks = list(map(int, input().split()))

_dict = {}

for card in cards:
    _dict[card] = 0  # 아무 값이나 매핑한 거임

for check in checks:
    if check not in _dict:
        print(0, end=" ")
    else:
        print(1, end=" ")
