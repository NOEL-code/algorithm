import sys
input = sys.stdin.readline

n = int(input())

for i in range(n):
    s = input().strip()
    
    print(s[0], s[len(s) -1])