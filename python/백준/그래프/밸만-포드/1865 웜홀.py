import sys
input = sys.stdin.readline

TC = int(input())

for _ in range(TC):
    N, M, W = map(int, input().split())
    
    edges = []
    
    # 도로 (양방향)
    for _ in range(M):
        S, E, T = map(int, input().split())
        edges.append((S, E, T))
        edges.append((E, S, T))
    
    # 웜홀 (단방향, 음수)
    for _ in range(W):
        S, E, T = map(int, input().split())
        edges.append((S, E, -T))
    
    dist = [0] * (N + 1)  # ⭐ 핵심
    
    has_cycle = False
    
    for i in range(N):
        for u, v, w in edges:
            if dist[v] > dist[u] + w:
                dist[v] = dist[u] + w
                
                if i == N - 1:
                    has_cycle = True
                    break
        
        if has_cycle:
            break
    
    print("YES" if has_cycle else "NO")