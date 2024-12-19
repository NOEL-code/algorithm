import sys
input = sys.stdin.readline

t = int(input())

for _ in range(t):
    n = int(input())
    wardrobe = {}
    
    for _ in range(n):
        name, clothes_type = input().split()
        
        if clothes_type in wardrobe:
            wardrobe[clothes_type].append(name)  # 단일 리스트에 추가
        else:
            wardrobe[clothes_type] = [name]  # 새 리스트로 초기화

    count = 1
    
    # 의상 종류별로 조합 계산
    for k in wardrobe:
        count *= (len(wardrobe[k]) + 1)  # 각 종류별로 '선택 안함' 포함
    print(count - 1)  # 모든 의상을 입지 않는 경우 제외
    
    
    