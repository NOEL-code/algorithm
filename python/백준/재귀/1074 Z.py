import sys
input = sys.stdin.readline

N, r, c = map(int, input().split())

count = 0  # 방문 순서

def search(x, y, size):
    global count
    # 크기가 1x1이 되면 값 출력
    if size == 1:
        if x == r and y == c:
            print(count)
            exit()  # 프로그램 종료
        count += 1
        return

    # 영역을 4분할
    new_size = size // 2

    # 각 영역의 시작점에서 재귀 호출 (Z 모양 순서)
    if r < x + new_size and c < y + new_size:  # 왼쪽 위
        search(x, y, new_size)
    elif r < x + new_size and c >= y + new_size:  # 오른쪽 위
        count += new_size * new_size  # 이전 영역 크기만큼 count 증가
        search(x, y + new_size, new_size)
    elif r >= x + new_size and c < y + new_size:  # 왼쪽 아래
        count += 2 * new_size * new_size  # 이전 영역 크기만큼 count 증가
        search(x + new_size, y, new_size)
    else:  # 오른쪽 아래
        count += 3 * new_size * new_size  # 이전 영역 크기만큼 count 증가
        search(x + new_size, y + new_size, new_size)

# 탐색 시작
search(0, 0, 2 ** N)