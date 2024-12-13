import sys
input = sys.stdin.readline

n, b = input().split()

result = 0

for i in range(len(n)):
    
    if type(n[i]) == int:
        result += int(n[i]) * pow(int(b), len(b) - i - 1)
    else:
        result += (ord(n[i]) - 55) * pow(int(b), len(n) - i - 1)


print(result)