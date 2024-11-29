# 입력
n = int(input())
data = input().split()

# 초기 위치 설정
x, y = 1, 1

# 방향 정의 (L, R, U, D 순서로)
dx = [0, 0, -1, 1]  # x 좌표 변화
dy = [-1, 1, 0, 0]  # y 좌표 변화
move_types = ['L', 'R', 'U', 'D']

# 명령어에 따른 이동
for a in data:
    for i in range(4):
        if a == move_types[i]:  # 현재 명령어에 해당하는 방향 찾기
            nx = x + dx[i]
            ny = y + dy[i]
            if 1 <= nx <= n and 1 <= ny <= n:  # 격자를 벗어나지 않는 경우만 이동
                x, y = nx, ny

# 최종 위치 출력
print(x, y)