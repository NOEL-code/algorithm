"""정렬된 배열의 이진 탐색과 답을 찾는 매개 변수 탐색."""

from bisect import bisect_left, bisect_right


def binary_search(values, target):
    """오름차순 정렬 필수. 일치하는 인덱스 하나, 없으면 -1. O(log N)."""
    left, right = 0, len(values) - 1

    while left <= right:
        mid = (left + right) // 2
        if values[mid] == target:
            return mid
        if values[mid] < target:
            left = mid + 1
        else:
            right = mid - 1

    return -1


def lower_bound(values, target):
    """target 이상인 첫 위치. 모두 작으면 len(values). O(log N)."""
    left, right = 0, len(values)  # [left, right) 범위

    while left < right:
        mid = (left + right) // 2
        if values[mid] < target:
            left = mid + 1
        else:
            right = mid

    return left


def first_true(left, right, check):
    """정수 닫힌 구간 [left, right]에서 처음 True가 되는 값.

    check 결과가 False ... False, True ... True 순서여야 한다.
    답이 없으면 최초 right + 1. O(log(범위 크기) * check 비용).
    """
    answer = right + 1
    while left <= right:
        mid = (left + right) // 2
        if check(mid):
            answer = mid
            right = mid - 1  # 가능한 값 중 더 작은 답을 찾는다.
        else:
            left = mid + 1
    return answer


def max_cable_length(cables, required):
    """양의 정수 길이 cables로 required(양수)개 이상 만드는 최대 길이.

    길이가 길수록 만들 수 있는 개수는 감소한다: True ... False.
    불가능하거나 cables가 비어 있으면 0. O(N log(max(cables))).
    """
    left, right = 1, max(cables, default=0)  # 0으로 나누지 않게 1부터
    answer = 0

    while left <= right:
        mid = (left + right) // 2
        count = sum(length // mid for length in cables)
        if count >= required:
            answer = mid
            left = mid + 1  # 가능한 값 중 더 큰 답을 찾는다.
        else:
            right = mid - 1

    return answer


if __name__ == "__main__":
    values = [1, 2, 2, 2, 5, 8]
    print(binary_search(values, 5))  # 4
    print(lower_bound(values, 2))   # 1
    print(bisect_left(values, 2), bisect_right(values, 2))  # 1 4
    print(bisect_right(values, 2) - bisect_left(values, 2))  # 개수: 3
    print(first_true(0, 100, lambda x: x * x >= 30))  # 6
    print(max_cable_length([802, 743, 457, 539], 11))  # 200
