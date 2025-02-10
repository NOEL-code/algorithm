import sys

input = sys.stdin.readline

N, M, B = map(int, input().split())
graph = [list(map(int, input().split())) for _ in range(N)]

time = float('inf')  # 최솟값을 찾기 위해 무한대 값 설정
glevel = 0  # 최적의 높이 저장

# 가능한 모든 높이 (0 ~ 256) 검사
for height in range(257):
    taken_block = 0  # 제거할 블록 개수
    used_block = 0   # 추가할 블록 개수

    # 땅을 순회하면서 블록 작업량 계산
    for x in range(N):
        for y in range(M):
            if graph[x][y] > height:
                taken_block += graph[x][y] - height  # 블록 제거 (인벤토리에 추가)
            else:
                used_block += height - graph[x][y]  # 블록 추가 (인벤토리에서 사용)

    # 블록이 부족한 경우 -> 이 높이는 만들 수 없음
    if used_block > taken_block + B:
        continue  # 다음 높이 검사

    # 총 걸리는 시간 계산
    temp_time = (taken_block * 2) + used_block

    # 최적 높이 업데이트 (시간이 작거나, 시간이 같으면 더 높은 높이 선택)
    if temp_time <= time:
        time = temp_time
        glevel = height

# 최종 결과 출력
print(time, glevel)