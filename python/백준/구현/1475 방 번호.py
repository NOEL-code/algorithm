nums = input().strip()

cnt = [0] * 10
for ch in nums:
    cnt[int(ch)] += 1

# 6과 9는 서로 호환 → 합친 뒤 올림 나눗셈
cnt[6] = cnt[9] = (cnt[6] + cnt[9] + 1) // 2

print(max(cnt))