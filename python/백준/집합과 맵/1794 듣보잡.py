import sys

input = sys.stdin.readline

N, M = map(int, input().split())

never_heard = set()
never_seen = set()
for i in range(N):
    never_heard.add(input().rstrip())

for i in range(M):
    never_seen.add(input().rstrip())

never_heard_seen = never_heard & never_seen
print(len(never_heard_seen))

for name in sorted(never_heard_seen):
    print(name)