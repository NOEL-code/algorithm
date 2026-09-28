"""백트래킹: 선택 → 재귀 → 원상복구. 조건을 만족하는 후보만 탐색한다."""


def permutations(n, length):
    """1~n에서 중복 없이 length개를 순서 있게 뽑는다. 0 <= length <= n.

    결과 복사를 포함해 O(length * n! / (n-length)!) 규모이므로 작은 입력용.
    """
    result = []
    path = []
    used = [False] * (n + 1)

    def dfs():
        if len(path) == length:
            result.append(path[:])  # 복사하지 않으면 같은 리스트를 공유한다.
            return

        for value in range(1, n + 1):
            if used[value]:
                continue

            used[value] = True
            path.append(value)
            dfs()
            path.pop()
            used[value] = False  # 다른 경로에서 다시 선택할 수 있게 복구

    dfs()
    return result


def combinations(n, length):
    """1~n에서 length개를 순서 없이 뽑는다. 0 <= length <= n."""
    result = []
    path = []

    def dfs(start):
        if len(path) == length:
            result.append(path[:])
            return

        needed = length - len(path)
        # 남은 수로 length개를 채울 수 없는 가지는 탐색하지 않는다.
        for value in range(start, n - needed + 2):
            path.append(value)
            dfs(value + 1)  # 다음 수는 현재 수보다 크게: 순서 중복 제거
            path.pop()

    dfs(1)
    return result


if __name__ == "__main__":
    print(permutations(3, 2))  # [[1, 2], [1, 3], [2, 1], [2, 3], [3, 1], [3, 2]]
    print(combinations(3, 2))  # [[1, 2], [1, 3], [2, 3]]
    # 단순 나열은 itertools.permutations / combinations로도 가능하다.
    # 후보가 많으면 result에 전부 저장하지 말고 답 갱신/출력/생성을 고려한다.
