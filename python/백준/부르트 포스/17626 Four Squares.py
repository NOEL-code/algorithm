import math

n = int(input())
d = [0] * 50001

def solution(n):
    if int(math.sqrt(n)) ** 2 == n:
        print(1)
        exit(0)
        
    for i in range(1, int(math.sqrt(n)) + 1):
        if int(math.sqrt(n - i * i)) ** 2 == (n - i * i):
            print(2)
            exit(0)
    
    for i in range(1, int(math.sqrt(n)) + 1):
        for j in range(1, int(math.sqrt(n - i * i)) + 1):
            if int(math.sqrt(n - i * i - j * j)) ** 2 == (n - i * i - j * j):
                print(3)
                exit(0)
    
    print(4)
    
solution(n)
    