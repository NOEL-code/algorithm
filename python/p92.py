n, m, k = map(int, input().split())

data = list(map(int, input().split()))

data.sort(reverse=True)

result = 0

largest = data[0]
small = data[1]

count = m // (k + 1)
remaining_count = m % (k + 1)

result = (count  * ((largest * k) + small)) + (largest * remaining_count)

print(result)
