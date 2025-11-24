import sys

input = sys.stdin.readline

H, W = map(int, input().split())
block = list(map(int, input().split()))

water = 0

for i in range(1, W-1):
    left_max = max(block[:i])
    right_max = max(block[i+1:])

    min_block = min(left_max, right_max)
    
    if min_block - block[i] > 0:
        water += min_block - block[i]

print(water) 

