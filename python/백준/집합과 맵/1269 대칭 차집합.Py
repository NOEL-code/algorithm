a_len, b_len = map(int, input().split())

a_set = set(map(int, input().split()))
b_set = set(map(int, input().split()))

diff_a = a_set - b_set
diff_b = b_set - a_set

print(len(diff_a) + len(diff_b))