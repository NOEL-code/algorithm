S = input()
T = list(input())

while len(T) > len(S):
    if T[-1] == 'A':
        T.pop()
    else:
        T.pop()
        T.reverse()

if ''.join(T) == S:
    print(1)
else:
    print(0)