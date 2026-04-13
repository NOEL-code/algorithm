#include <iostream>
#include <deque>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, w, l;
    cin >> n >> w >> l;

    deque<int> weight;
    for (int i = 0; i < n; ++i) {
        int x;
        cin >> x;
        weight.push_back(x);
    }

    deque<pair<int, int>> bridge;
    int time = 0;
    int bridge_weight = 0;

    bridge.push_back({weight.front(), 1});
    bridge_weight += weight.front();
    weight.pop_front();
    time += 1;

    while (!bridge.empty()) {
        for (auto& truck : bridge) truck.second += 1;

        if (bridge.front().second > w) {
            bridge_weight -= bridge.front().first;
            bridge.pop_front();
        }

        if (!weight.empty() && bridge_weight + weight.front() <= l) {
            bridge.push_back({weight.front(), 1});
            bridge_weight += weight.front();
            weight.pop_front();
        }

        time += 1;
    }

    cout << time << "\n";
    return 0;
}
