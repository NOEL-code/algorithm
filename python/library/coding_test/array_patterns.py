"""누적 합, 투 포인터, 슬라이딩 윈도우. 배열 인덱스는 0부터 시작한다."""


def prefix_sum(values):
    """O(N) 전처리. [left, right) 구간 합 = prefix[right] - prefix[left]."""
    prefix = [0]
    for value in values:
        prefix.append(prefix[-1] + value)
    return prefix


def find_pair(values, target):
    """오름차순 배열에서 합이 target인 서로 다른 두 위치, 없으면 None. O(N)."""
    left, right = 0, len(values) - 1
    while left < right:
        total = values[left] + values[right]
        if total == target:
            return left, right
        if total < target:
            left += 1
        else:
            right -= 1
    return None


def min_subarray_length(values, target):
    """양수 배열에서 합이 target(양수) 이상인 최소 연속 구간 길이. O(N).

    답이 없으면 0. 음수가 있으면 구간 합의 단조성이 깨져 사용할 수 없다.
    """
    left = 0
    total = 0
    answer = len(values) + 1

    for right, value in enumerate(values):
        total += value
        while total >= target:
            answer = min(answer, right - left + 1)
            total -= values[left]
            left += 1

    return answer if answer <= len(values) else 0


def max_window_sum(values, k):
    """길이가 정확히 k인 연속 구간의 최대 합. 1 <= k <= len(values). O(N)."""
    if not 1 <= k <= len(values):
        raise ValueError("k는 1 이상 배열 길이 이하여야 합니다.")

    total = sum(values[:k])
    answer = total  # 음수 배열도 가능하므로 0으로 초기화하면 안 된다.
    for right in range(k, len(values)):
        total += values[right] - values[right - k]
        answer = max(answer, total)
    return answer


if __name__ == "__main__":
    prefix = prefix_sum([3, 1, 4, 1, 5])
    print(prefix[4] - prefix[1])  # 인덱스 1~3의 합: 6
    print(find_pair([1, 2, 4, 7, 11], 9))  # (1, 3)
    print(min_subarray_length([2, 3, 1, 2, 4, 3], 7))  # 2
    print(max_window_sum([-5, -2, -3, -1], 2))  # -4
