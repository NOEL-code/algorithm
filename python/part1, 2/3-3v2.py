n, m, k = map(int, input().split())

# n 숫자 갯수, m 횟수, k 반복횟수

data = list(map(int, input().split()))

data.sort()

first = data[n-1]
second = data[n-2]

if m % k == 0:
    result = ((first * k) + second) * (m / (k + 1)) 
else:
    result = ((first * k) + second) * (m / (k + 1)) + first * (m % (k + 1))

print(result)

