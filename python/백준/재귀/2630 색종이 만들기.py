import sys
input = sys.stdin.readline

# 입력 받기
N = int(input().strip())
paper = [list(map(int, input().split())) for _ in range(N)]

# 하얀색(0)과 파란색(1) 색종이의 개수를 저장
white_count = 0
blue_count = 0

def divide(x, y, size):
    global white_count, blue_count
    
    # 첫 번째 칸의 색을 기준으로 설정
    color = paper[x][y]
    
    # 전체 영역이 같은 색인지 확인
    for i in range(x, x + size):
        for j in range(y, y + size):
            if paper[i][j] != color:
                # 색이 다른 경우, 4개의 영역으로 분할
                new_size = size // 2
                divide(x, y, new_size)             # 왼쪽 위
                divide(x, y + new_size, new_size)  # 오른쪽 위
                divide(x + new_size, y, new_size) # 왼쪽 아래
                divide(x + new_size, y + new_size, new_size) # 오른쪽 아래
                return
    
    # 모든 칸이 같은 색인 경우
    if color == 0:
        white_count += 1
    else:
        blue_count += 1

# 전체 영역에 대해 분할 시작
divide(0, 0, N)

# 결과 출력
print(white_count)
print(blue_count)