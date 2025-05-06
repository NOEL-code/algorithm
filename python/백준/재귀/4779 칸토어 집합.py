def cantor_set(start, end):
    if end == 1:
        return
    
    third = end // 3
    for i in range(start + third, start + 2 * third):
        string[i] = ' '
    
    cantor_set(start, third)
    cantor_set(start + 2 * third, third)

try:
    while True:
        N = input()
        if N.strip() == "":
            break
        N = int(N)
        string = ['-'] * (3 ** N)
        cantor_set(0, 3 ** N)
        print(''.join(string))
except EOFError:
    pass