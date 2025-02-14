import sys
from collections import deque

input = sys.stdin.readline

def bfs(A, B):
    deq = deque([(A, "")])  # 시작 숫자와 명령어 저장
    visited = [False] * 10000  # 방문 체크 배열
    visited[A] = True  # 시작점 방문 처리
    
    while deq:
        num, command = deq.popleft()
        
        if num == B:
            return command  # 최단 경로 반환

        # D 연산 (2배, 10000으로 나눈 나머지)
        d = (num * 2) % 10000
        if not visited[d]:
            visited[d] = True
            deq.append((d, command + "D"))

        # S 연산 (1 감소, 0이면 9999)
        s = 9999 if num == 0 else num - 1
        if not visited[s]:
            visited[s] = True
            deq.append((s, command + "S"))

        # L 연산 (왼쪽으로 회전) -> 공식 적용
        l = num // 1000 + (num % 1000) * 10
        if not visited[l]:
            visited[l] = True
            deq.append((l, command + "L"))

        # R 연산 (오른쪽으로 회전) -> 공식 적용
        r = num // 10 + (num % 10) * 1000
        if not visited[r]:
            visited[r] = True
            deq.append((r, command + "R"))

# 입력 처리
T = int(input())
for _ in range(T):
    A, B = map(int, input().split())
    print(bfs(A, B))