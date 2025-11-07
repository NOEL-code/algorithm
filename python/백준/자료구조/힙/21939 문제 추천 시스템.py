import sys, heapq
input = sys.stdin.readline

# 두 힙과 현재 상태 딕셔너리 사용
min_heap, max_heap = [], []
level = {}  # level[P] = L (현재 유효 난이도)

N = int(input())
for _ in range(N):
    P, L = map(int, input().split())  # 입력은 P L 순서!
    level[P] = L
    heapq.heappush(min_heap, (L, P))
    heapq.heappush(max_heap, (-L, -P))

M = int(input())

for _ in range(M):
    parts = input().split()
    cmd = parts[0]

    if cmd == 'add':
        P = int(parts[1])
        L = int(parts[2])
        level[P] = L
        heapq.heappush(min_heap, (L, P))
        heapq.heappush(max_heap, (-L, -P))

    elif cmd == 'solved':
        P = int(parts[1])
        if P in level:
            del level[P]  # lazy deletion: 실제 힙에선 나중에 걸러냄

    else:  # recommend x
        x = int(parts[1])
        if x == 1:
            # 가장 어려운: L 최대, 동률이면 P 최대
            while max_heap:
                Lneg, Pneg = max_heap[0]
                L = -Lneg; P = -Pneg
                if P in level and level[P] == L:
                    print(P)
                    break
                heapq.heappop(max_heap)  # 무효 항목 버림
        else:
            # 가장 쉬운: L 최소, 동률이면 P 최소
            while min_heap:
                L, P = min_heap[0]
                if P in level and level[P] == L:
                    print(P)
                    break
                heapq.heappop(min_heap)  # 무효 항목 버림
