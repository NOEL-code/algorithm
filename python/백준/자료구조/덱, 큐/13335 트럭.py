from collections import deque

n, w, l = map(int, input().split())
weight = list(map(int, input().split()))

bridge = deque()
time = 0
bridge_weight = 0

bridge.append([weight[0], 1])
bridge_weight += weight[0]
weight.pop(0)
time += 1

while bridge:
    for truck in bridge:
        truck[1] += 1

    if bridge[0][1] > w:
        bridge_weight -= bridge[0][0]
        bridge.popleft()
    
    if weight and bridge_weight + weight[0] <= l:
        bridge.append([weight[0], 1])
        bridge_weight += weight[0]
        weight.pop(0)
    
    time += 1

print(time)
            
    