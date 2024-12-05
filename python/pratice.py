import sys
input = sys.stdin.readline

s = input().strip()

data = [0] * 26  # 알파벳 26자에 대한 배열 생성

for char in s:
    if 'a' <= char <= 'z':  # 소문자 알파벳만 처리
        data[ord(char) - 97] += 1
