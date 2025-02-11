from collections import deque

def bfs(A, B):
    queue = deque([(A, 1)])  # (현재 값, 연산 횟수)
    
    while queue:
        num, count = queue.popleft()
        
        # 목표 값 B에 도달한 경우 정답 반환
        if num == B:
            return count
        
        # 1. 2를 곱한 값 추가
        next_num = num * 2
        if next_num <= B:
            queue.append((next_num, count + 1))
        
        # 2. 1을 오른쪽에 추가한 값 추가
        next_num = int(str(num) + "1")  # 문자열로 변환 후 붙이기
        if next_num <= B:
            queue.append((next_num, count + 1))
    
    return -1  # B를 만들 수 없는 경우

# 입력 받기
A, B = map(int, input().split())

# 결과 출력
print(bfs(A, B))