n = int(input())  # 배열의 크기 입력
array = list(map(int, input().split()))  # 배열 입력

# 좌표 압축 수행
array_s = sorted(set(array))  # 중복 제거 후 정렬
dic = {array_s[i]: i for i in range(len(array_s))}  # 값에 순위를 매핑

# 압축된 값 출력
for a in array:
    print(dic[a], end=" ")