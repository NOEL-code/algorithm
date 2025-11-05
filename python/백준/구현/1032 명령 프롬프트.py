n = int(input())

base_string = list(input().strip())  # 문자열 → 리스트로 변환

for _ in range(n-1):
    compare_string = str(input())
    for i in range(len(base_string)):
        if base_string[i] != compare_string[i]:
            base_string[i] = "?"

print("".join(base_string))  # 리스트 → 문자열로 다시 합침    