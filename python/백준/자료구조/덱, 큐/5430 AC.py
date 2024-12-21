import sys
from collections import deque

input = sys.stdin.readline

T = int(input())  # 테스트 케이스 수 입력

for _ in range(T):
    # 입력 처리
    p = input().strip()  # 명령어
    n = int(input())  # 배열 크기
    arr = input().strip()[1:-1]  # 배열 형태의 문자열
    
    # 배열 초기화
    if n == 0:
        queue = deque()  # 빈 배열일 경우
    else:
        queue = deque(arr.split(','))  # 배열로 변환
    
    reverse_flag = False  # 뒤집기 여부를 추적
    error_flag = False  # 에러 발생 여부 추적
    
    # 명령어 처리
    for cmd in p:
        if cmd == 'R':
            reverse_flag = not reverse_flag  # 뒤집기 상태 변경
        elif cmd == 'D':
            if queue:
                if reverse_flag:
                    queue.pop()  # 뒤집힌 상태라면 뒤에서 제거
                else:
                    queue.popleft()  # 정상 상태라면 앞에서 제거
            else:
                error_flag = True  # 에러 발생
                break
    
    # 결과 출력
    if error_flag:
        print("error")
    else:
        if reverse_flag:
            queue = reversed(queue)  # 결과적으로 뒤집어서 출력
        print('[' + ",".join(queue) + ']')
