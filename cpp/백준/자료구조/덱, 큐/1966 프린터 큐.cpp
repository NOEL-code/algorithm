#include <iostream>
#include <deque>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int N, M;
        cin >> N >> M;

        deque<pair<int, int>> queue;
        for (int i = 0; i < N; ++i) {
            int p;
            cin >> p;
            queue.push_back({p, i});
        }

        int count = 0;
        while (!queue.empty()) {
            auto current = queue.front();
            queue.pop_front();

            bool hasHigher = false;
            for (auto& it : queue) {
                if (it.first > current.first) { hasHigher = true; break; }
            }

            if (hasHigher) {
                queue.push_back(current);
            } else {
                ++count;
                if (current.second == M) {
                    cout << count << "\n";
                    break;
                }
            }
        }
    }
    return 0;
}
