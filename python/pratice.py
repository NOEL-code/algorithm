import sys
input = sys.stdin.readline

s = input().strip()

data = ["ABC", "DEF", "GHI", "JKL", "MNO", "PQRS", "TUV", "WXYZ"]
result = 0


for i in range(len(s)):
    for j in data:
        if s[i] in j:
            result += data.index(j) + 3

print(result)