import sys
input = sys.stdin.readline

n = int(input())

start = 1 

for i in range(0, n):
    start = start + (6 * i)
    
    if start > n:
        print(i + 1)
        break
