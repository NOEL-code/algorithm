# 코딩테스트 Python 기본 구현 모음

각 파일은 표준 라이브러리만 사용하며, 직접 실행하면 예제 결과를 출력합니다. 필요한 함수와 import를 제출 코드에 복사하고 문제의 입력 형식에 맞게 연결하세요.

```bash
python3 python/library/coding_test/dfs.py
python3 python/library/coding_test/bfs.py
python3 python/library/coding_test/dijkstra.py
python3 python/library/coding_test/binary_search.py
```

**그래프 정점은 1~N, 배열과 격자 좌표는 0부터 시작합니다.** 그래프의 `graph[0]`과 결과 배열의 0번 칸은 사용하지 않습니다. 각 파일 아래의 `if __name__ == "__main__":`에 입력 예와 예상 출력이 있습니다.

## 무엇부터 복습할까?

먼저 DFS → BFS → 다익스트라 → 이진 탐색을 직접 작성해 보고, 백트래킹과 배열 패턴을 복습하세요. 이후 유니온 파인드·위상 정렬·DP까지 확인하면 됩니다.

| 문제에서 보이는 조건 | 사용할 구현 | 핵심 |
|---|---|---|
| 연결 여부, 연결 요소, 깊이 우선 탐색 | [dfs.py](dfs.py) | 재귀 / 스택, 방문 처리 |
| 간선 비용이 같고 최단 거리 필요 | [bfs.py](bfs.py) | `deque`, 큐에 넣을 때 방문 처리 |
| 격자에서 상하좌우 최단 이동 | [bfs.py](bfs.py)의 `grid_bfs` | 범위·벽·방문 여부 확인 |
| 간선 비용이 다르고 모두 0 이상 | [dijkstra.py](dijkstra.py) | `(거리, 정점)` 최소 힙, 거리 갱신 |
| 정렬된 데이터에서 값·경계 찾기 | [binary_search.py](binary_search.py) | 중복은 `bisect_left/right` |
| 가능한 최소값/최대값, 판정 결과가 단조로움 | [binary_search.py](binary_search.py)의 `first_true`, `max_cable_length` | 답을 가정하고 가능 여부 검사 |
| 작은 입력에서 순열·조합·선택 탐색 | [backtracking.py](backtracking.py) | 선택 → 재귀 → 복구 |
| 집합 합치기, 무방향 간선 추가 시 사이클 확인 | [union_find.py](union_find.py) | 대표 정점 비교 |
| 모든 정점을 최소 비용으로 연결 | [union_find.py](union_find.py)의 `kruskal` | 비용 순 정렬 + 유니온 파인드 |
| 선행 작업·의존성·수강 순서 | [topological_sort.py](topological_sort.py) | 진입 차수가 0인 정점부터 처리 |
| 구간 합 질의가 반복됨 | [array_patterns.py](array_patterns.py)의 `prefix_sum` | 전처리 O(N), 질의 O(1) |
| 정렬 배열의 두 수 합 | [array_patterns.py](array_patterns.py)의 `find_pair` | 양 끝 포인터 |
| 양수 연속 구간 합 / 길이가 고정된 구간 | [array_patterns.py](array_patterns.py) | 투 포인터 / 슬라이딩 윈도우 |
| 최소 동전 수, 0/1 배낭, LIS 길이 | [dp.py](dp.py) | 상태 정의, 초기값, 갱신 순서 |

V는 정점 수, E는 간선 수입니다. DFS/BFS/위상 정렬은 O(V+E), 격자 BFS는 O(행×열), 이진 탐색은 O(log N)입니다. 각 함수 주석에 전제 조건과 시간 복잡도를 적었습니다.

## 문제 입력 연결하기

일반 그래프 입력 예시입니다. `n m` 다음에 간선이 m줄 들어온다고 가정합니다.

```python
import sys
input = sys.stdin.readline

n, m = map(int, input().split())
graph = [[] for _ in range(n + 1)]
for _ in range(m):
    a, b = map(int, input().split())
    graph[a].append(b)
    graph[b].append(a)  # 방향 그래프라면 이 줄 삭제

# 작은 번호부터 방문하라는 조건이 있을 때만 정렬
for neighbors in graph:
    neighbors.sort()

# 위에 복사해 둔 bfs 함수를 사용
distance = bfs(graph, 1)
print(distance[n])  # 1번에서 n번까지의 최소 간선 수, 도달 불가능하면 -1
```

다익스트라는 간선 저장 모양이 다릅니다.

```python
graph = [[] for _ in range(n + 1)]
for _ in range(m):
    a, b, cost = map(int, input().split())
    graph[a].append((b, cost))
    # 무방향이면 graph[b].append((a, cost))도 추가

distance = dijkstra(graph, 1)
print(distance[n] if distance[n] != float("inf") else -1)
```

격자는 입력의 공백 유무를 구분하세요.

```python
# 10101처럼 붙어 있는 숫자 격자
board = [list(map(int, input().strip())) for _ in range(n)]
# 1 0 1 0 1처럼 공백이 있는 숫자 격자
board = [list(map(int, input().split())) for _ in range(n)]
```

두 방식 중 문제에 맞는 한 줄만 사용합니다.

## 시험 직전 확인할 실수

- DFS는 일반 그래프의 최단 거리를 보장하지 않습니다. 동일 비용은 BFS, 음수 없는 가중치는 다익스트라를 사용합니다.
- 방문 처리는 일반 탐색에서는 유지하고, 백트래킹에서는 해당 선택을 되돌릴 때 복구합니다. 모든 경로 탐색은 일반 DFS보다 훨씬 비쌀 수 있습니다.
- 재귀 DFS는 깊은 입력에서 `RecursionError`가 날 수 있습니다. `sys.setrecursionlimit(...)`는 호출 깊이 제한만 바꾸므로 큰 입력에서는 스택 버전을 우선 고려하세요.
- BFS의 `distance[start] = 0`은 이동 횟수 기준입니다. 시작 칸을 포함하는 문제인지 확인하세요.
- 다익스트라에서 방문 배열로 처음 발견한 경로를 확정하지 않습니다. 거리 갱신과 오래된 힙 항목 건너뛰기가 핵심입니다.
- 이진 탐색은 정렬 또는 판정의 단조성이 필요합니다. `left <= right` 버전과 `[left, right)` 버전의 경계 갱신을 섞지 마세요.
- `bisect_left`는 target 이상인 첫 위치, `bisect_right`는 target 초과인 첫 위치입니다. 반환값이 배열 길이일 수 있습니다.
- `[[0] * m] * n`은 행을 공유합니다. `[[0] * m for _ in range(n)]`으로 만드세요.
- 투 포인터로 연속 구간 합을 줄이는 예제는 양수 배열용입니다. 음수가 섞이면 그대로 적용할 수 없습니다.
- DP에서는 초기값 0이 가능한 상태인지 확인하세요. 도달 불가능한 최소 비용은 `inf`로 구분합니다.
- 빈 입력, 정점 1개, 도달 불가능, 중복 값, 전부 음수인 배열 등 문제에서 허용하는 경계 조건을 확인하세요.

## 자주 쓰는 표준 라이브러리

```python
from collections import deque, Counter, defaultdict
from heapq import heappush, heappop
from bisect import bisect_left, bisect_right
from itertools import combinations, permutations
from math import gcd, isqrt

counts = Counter([1, 1, 2])              # 값별 개수
groups = defaultdict(list)              # 키마다 별도 리스트
pairs = list(combinations([1, 2, 3], 2))  # [(1, 2), (1, 3), (2, 3)]
```

순열·조합은 결과 수가 빠르게 늘어납니다. 모두 저장할 필요가 없다면 `list(...)` 대신 반복하면서 처리하세요.
