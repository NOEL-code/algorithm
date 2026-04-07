import sys

S = input()
T = input()

def solve(t):
    if len(t) == len(S):
        if t == S:
            print(1)
            sys.exit()
        return

    if t[-1] == 'A':
        solve(t[:-1])

    if t[0] == 'B':
        solve(t[1:][::-1])

solve(T)
print(0)