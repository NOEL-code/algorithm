# 톱니바퀴 입력
wheel = [list(map(int, input().strip())) for _ in range(4)]
K = int(input())

# 회전 함수
def rotate(w, d):
    if d == 1:  # 시계 방향
        return [w[-1]] + w[:-1]
    else:       # 반시계 방향
        return w[1:] + [w[0]]

# 명령 처리
for _ in range(K):
    num, direction = map(int, input().split())
    num -= 1  # 0-index로 변환

    # 각 톱니의 회전 방향 저장
    dirs = [0] * 4
    dirs[num] = direction

    # 왼쪽 전파
    for i in range(num - 1, -1, -1):
        if wheel[i][2] != wheel[i + 1][6]:
            dirs[i] = -dirs[i + 1]
        else:
            break

    # 오른쪽 전파
    for i in range(num + 1, 4):
        if wheel[i - 1][2] != wheel[i][6]:
            dirs[i] = -dirs[i - 1]
        else:
            break

    # 실제 회전 적용
    for i in range(4):
        if dirs[i] != 0:
            wheel[i] = rotate(wheel[i], dirs[i])

# 점수 계산
point = 0
for i in range(4):
    if wheel[i][0] == 1:
        point += (1 << i)  # 2^i

print(point)