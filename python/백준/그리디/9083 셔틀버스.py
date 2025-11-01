import sys
from datetime import datetime, timedelta
import heapq

input = sys.stdin.readline
T = int(input())

for _ in range(T):
    D = int(input()) 
    A = int(input())
    bus=[]
    for _ in range(A):
        tmp = datetime.strptime(input().strip(), '%H:%M')
        bus.append((tmp, 'S',tmp+timedelta(minutes=D)) )
    B = int(input())
    for _ in range(B):
        tmp = datetime.strptime(input().strip(), '%H:%M')
        bus.append((tmp, 'T',tmp+timedelta(minutes=D)) )

    bus.sort(key=lambda x: x[0])

    school = []   
    terminal = [] 
    cnt=0

    for time, bus_type, ready_time in bus:
        if bus_type == 'S':
            temp = []
            while school and school[0] <= time:
                temp.append(heapq.heappop(school))
            if temp:
                temp.remove(max(temp))
                for t in temp:
                    heapq.heappush(school, t)
            else:
                if terminal and terminal[0] + timedelta(minutes=D) <= time:
                    heapq.heappop(terminal)
                else:
                    cnt += 1
            heapq.heappush(terminal, ready_time)
        else: 
            temp = []
            while terminal and terminal[0] <= time:
                temp.append(heapq.heappop(terminal))
            if temp:
                temp.remove(max(temp))
                for t in temp:
                    heapq.heappush(terminal, t)
            else:
                if school and school[0] + timedelta(minutes=D) <= time:
                    heapq.heappop(school)
                else:
                    cnt += 1
            heapq.heappush(school, ready_time)

    print(cnt)