#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
using namespace std;

const int INF = 1e9;

int n, m, r;
vector<int> items;
vector<vector<pair<int, int>>> graph;

int dijkstra(int start) {
    vector<int> distance(n, INF);
    distance[start] = 0;
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> q;
    q.push({0, start});

    while (!q.empty()) {
        auto [dist, now] = q.top();
        q.pop();

        if (distance[now] < dist) continue;

        for (auto [neighbor, weight] : graph[now]) {
            int cost = dist + weight;
            if (cost < distance[neighbor]) {
                distance[neighbor] = cost;
                q.push({cost, neighbor});
            }
        }
    }

    int total_items = 0;
    for (int i = 0; i < n; ++i) {
        if (distance[i] <= m) total_items += items[i];
    }
    return total_items;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m >> r;
    items.assign(n, 0);
    for (int i = 0; i < n; ++i) cin >> items[i];

    graph.assign(n, {});
    for (int i = 0; i < r; ++i) {
        int a, b, l;
        cin >> a >> b >> l;
        graph[a - 1].push_back({b - 1, l});
        graph[b - 1].push_back({a - 1, l});
    }

    int result = 0;
    for (int i = 0; i < n; ++i) {
        result = max(result, dijkstra(i));
    }

    cout << result << "\n";
    return 0;
}
