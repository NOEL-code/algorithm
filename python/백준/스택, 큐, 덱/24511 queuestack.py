from collections import deque
import sys
input = sys.stdin.readline

N = int(input())                    # queuestack을 구성하는 N개의 자료구조
A = list(map(int, input().split())) # 길이 N의 수열 A
B = list(map(int, input().split())) # 길이 N의 수열 B
M = int(input())                    # 삽입할 수열의 길이 M
C = list(map(int, input().split())) # 길이 M의 수열 C

queue = deque([])

for i in range(N):
  if A[i] == 0:
    queue.append(B[i])

for i in range(M):
  queue.appendleft(C[i])
  print(queue.pop(), end=' ')