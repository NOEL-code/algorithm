#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<pair<int, int>> lectures(N);
    for (int i = 0; i < N; ++i) {
        int start, end;
        cin >> start >> end;
        lectures[i] = {start, end};
    }
    sort(lectures.begin(), lectures.end());

    priority_queue<int, vector<int>, greater<int>> heap;
    heap.push(lectures[0].second);

    for (int i = 1; i < N; ++i) {
        int start = lectures[i].first;
        int end = lectures[i].second;

        if (heap.top() <= start) {
            heap.pop();
        }
        heap.push(end);
    }

    cout << heap.size() << "\n";
    return 0;
}
