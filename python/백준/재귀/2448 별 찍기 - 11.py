import sys
input = sys.stdin.readline

def star(n, r, c):
    if n == 1:
        return '*'
    
    # 현재 위치가 어느 블록에 속하는지
    block = n // 3
    if (r // block) == 1 and (c // block) == 1:
        return ' '  # 가운데 블록은 공백
    
    return star(block, r % block, c % block)

n = int(input())

result = []
for r in range(n):
    row = []
    for c in range(n):
        row.append(star(n, r, c))
    result.append(''.join(row))

print('\n'.join(result))