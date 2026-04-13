#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
using namespace std;

const int INF = 1e9;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n, d, c;
        cin >> n >> d >> c;

        vector<vector<pair<int, int>>> graph(n + 1);
        vector<int> distance(n + 1, INF);

        for (int i = 0; i < d; ++i) {
            int a, b, s;
            cin >> a >> b >> s;
            graph[b].push_back({a, s});
        }

        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> queue;
        queue.push({0, c});
        distance[c] = 0;

        while (!queue.empty()) {
            auto [dist, now] = queue.top();
            queue.pop();

            if (dist > distance[now]) continue;

            for (auto [next_node, cost] : graph[now]) {
                int new_cost = dist + cost;
                if (new_cost < distance[next_node]) {
                    distance[next_node] = new_cost;
                    queue.push({new_cost, next_node});
                }
            }
        }

        int count = 0;
        int max_time = 0;
        for (int i = 1; i <= n; ++i) {
            if (distance[i] != INF) {
                count += 1;
                max_time = max(max_time, distance[i]);
            }
        }

        cout << count << " " << max_time << "\n";
    }
    return 0;
}
