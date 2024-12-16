def recursion(s, l, r, cnt):
    if l >= r: 
        return [1, cnt]
    elif s[l] != s[r]: 
        return [0, cnt]
    else: 
        return recursion(s, l+1, r-1, cnt + 1)

def isPalindrome(s, cnt):
    return recursion(s, 0, len(s) - 1, cnt)

T = int(input())

for _ in range(T):
    s = input().strip()  # 입력 문자열
    result = isPalindrome(s, 1)  # 회문 여부와 재귀 호출 횟수 반환
    print(" ".join(map(str, result)))  # 리스트를 문자열로 변환하여 출력