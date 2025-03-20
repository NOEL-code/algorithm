from collections import deque

room_size = 4
person = [2, 3, 1, 5]

time_elapsed = 0

queue = deque([person.pop(0)])
for _ in range(room_size - 1):
    queue.append(0)

waiting_list = deque(person)

while queue:
    time_elapsed += 1
    
    for i in range(len(queue)):
        queue[i] -= 1
    
    if queue[0] <= 0:
        queue.popleft()
        
    if waiting_list and len(queue) < room_size:
        queue.append(waiting_list.popleft())
    
    queue.rotate(-1)
    
print(time_elapsed)