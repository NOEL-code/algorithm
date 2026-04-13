#include <iostream>
#include <queue>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> min_heap;
    priority_queue<pair<int, int>> max_heap;
    unordered_map<int, int> level;

    int N;
    cin >> N;
    for (int i = 0; i < N; ++i) {
        int P, L;
        cin >> P >> L;
        level[P] = L;
        min_heap.push({L, P});
        max_heap.push({L, P});
    }

    int M;
    cin >> M;

    while (M--) {
        string cmd;
        cin >> cmd;

        if (cmd == "add") {
            int P, L;
            cin >> P >> L;
            level[P] = L;
            min_heap.push({L, P});
            max_heap.push({L, P});
        } else if (cmd == "solved") {
            int P;
            cin >> P;
            level.erase(P);
        } else {
            int x;
            cin >> x;
            if (x == 1) {
                while (!max_heap.empty()) {
                    auto [L, P] = max_heap.top();
                    auto it = level.find(P);
                    if (it != level.end() && it->second == L) {
                        cout << P << "\n";
                        break;
                    }
                    max_heap.pop();
                }
            } else {
                while (!min_heap.empty()) {
                    auto [L, P] = min_heap.top();
                    auto it = level.find(P);
                    if (it != level.end() && it->second == L) {
                        cout << P << "\n";
                        break;
                    }
                    min_heap.pop();
                }
            }
        }
    }
    return 0;
}
