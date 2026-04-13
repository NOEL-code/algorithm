#include <iostream>
#include <deque>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<int> numbers(M);
    for (int i = 0; i < M; ++i) cin >> numbers[i];

    deque<int> dq;
    for (int i = 1; i <= N; ++i) dq.push_back(i);

    int count = 0;
    int idx = 0;
    while (idx < M) {
        if (dq.front() == numbers[idx]) {
            dq.pop_front();
            ++idx;
        } else {
            int pos = 0;
            for (int i = 0; i < (int)dq.size(); ++i) {
                if (dq[i] == numbers[idx]) { pos = i; break; }
            }
            if (pos <= (int)dq.size() / 2) {
                dq.push_back(dq.front());
                dq.pop_front();
                ++count;
            } else {
                dq.push_front(dq.back());
                dq.pop_back();
                ++count;
            }
        }
    }

    cout << count << "\n";
    return 0;
}
