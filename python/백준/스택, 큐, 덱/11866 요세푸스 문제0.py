from collections import deque

circle = deque()

n, k = map(int, input().split())

for i in range(1, n + 1):
    circle.append(i)

result = []
while circle:
    # k - 1번 왼쪽으로 회전
    circle.rotate(-(k - 1))
    # 가장 앞의 요소를 제거하고 결과 리스트에 추가
    result.append(circle.popleft())

# 결과 출력
print("<", end='')
print(", ".join(map(str, result)), end='')
print(">")