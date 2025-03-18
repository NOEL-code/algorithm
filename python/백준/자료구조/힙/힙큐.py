import heapq

n = 4
waiting_person_array = [2, 3, 1, 5]
queue = []
count = 0

heapq.heappush(queue, waiting_person_array.pop(0))

while queue:
    count += 1
    next_queue = []
    
    while queue:
        time = heapq.heappop(queue)
        time -= 1
        if time > 0:  
            next_queue.append(time)

    for person in next_queue:
        heapq.heappush(queue, person)
        
    if len(queue) < n:
        if waiting_person_array:
            heapq.heappush(queue, waiting_person_array.pop(0))
    
print(count)

from collections import deque

room_size = 4
waiting_person_array = [2, 3, 1, 5]
queue = deque()

count = 0
queue.append(waiting_person_array.pop(0))

while queue:
    count += 1
    
    queue.rotate(1)
    
