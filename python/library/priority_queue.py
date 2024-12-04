class PriorityQueue:
    def __init__(self):
        self.heap = []

    def push(self, value):
        """값을 힙에 삽입"""
        self.heap.append(value)  # 힙의 끝에 값을 추가
        self._heapify_up(len(self.heap) - 1)  # 힙 속성을 유지

    def pop(self):
        """최소값 반환 및 제거"""
        if len(self.heap) == 0:
            raise IndexError("pop from an empty priority queue")
        
        # 루트(최소값)를 제거하고, 마지막 요소를 루트로 이동
        root = self.heap[0]
        self.heap[0] = self.heap[-1]
        self.heap.pop()  # 마지막 요소 제거
        self._heapify_down(0)  # 힙 속성을 유지
        return root

    def peek(self):
        """최소값 확인 (제거하지 않음)"""
        if len(self.heap) == 0:
            raise IndexError("peek from an empty priority queue")
        return self.heap[0]

    def _heapify_up(self, index):
        """삽입 시 힙 속성 유지"""
        parent = (index - 1) // 2
        while index > 0 and self.heap[index] < self.heap[parent]:
            # 부모 노드와 교환
            self.heap[index], self.heap[parent] = self.heap[parent], self.heap[index]
            index = parent
            parent = (index - 1) // 2

    def _heapify_down(self, index):
        """삭제 시 힙 속성 유지"""
        child = 2 * index + 1  # 왼쪽 자식
        while child < len(self.heap):
            # 오른쪽 자식이 더 작으면 오른쪽 자식을 선택
            if child + 1 < len(self.heap) and self.heap[child + 1] < self.heap[child]:
                child += 1
            # 부모가 자식보다 작으면 종료
            if self.heap[index] <= self.heap[child]:
                break
            # 부모와 자식 교환
            self.heap[index], self.heap[child] = self.heap[child], self.heap[index]
            index = child
            child = 2 * index + 1