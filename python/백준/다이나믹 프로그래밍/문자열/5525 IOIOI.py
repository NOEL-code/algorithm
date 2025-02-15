N = int(input())
M = int(input())
S = input()

count = 0
pattern_length = 0

i = 0

while i < M - 2:
    if S[i:i+3] == "IOI":
        pattern_length += 1
        if pattern_length == N:
            count += 1
            pattern_length -= 1
        i += 2
    else:
        pattern_length = 0
        i += 1

print(count)