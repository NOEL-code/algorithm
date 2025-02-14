def max_fruit_length(N, fruits):
    from collections import defaultdict
    
    left = 0
    fruit_count = defaultdict(int)  # 과일의 개수를 저장하는 딕셔너리
    max_length = 0
    
    for right in range(N):
        fruit_count[fruits[right]] += 1  # 오른쪽 과일 추가
        
        # 과일 종류가 3개 이상이면 왼쪽 포인터 이동
        while len(fruit_count) > 2:
            fruit_count[fruits[left]] -= 1
            if fruit_count[fruits[left]] == 0:
                del fruit_count[fruits[left]]  # 과일 개수가 0이면 제거
            left += 1  # 왼쪽 포인터 이동
        
        # 현재 윈도우 크기 갱신
        max_length = max(max_length, right - left + 1)
    
    return max_length

# 입력 받기
N = int(input())
fruits = list(map(int, input().split()))

# 결과 출력
print(max_fruit_length(N, fruits))