import sys
input = sys.stdin.readline

N, M = map(int, input().split())

_dict = {}

for _ in range(N):
  site, password = input().split()
  _dict[site] = password

for _ in range(M):
  site = input().strip()
  
  print(str(_dict[site]))