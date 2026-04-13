#include <iostream>
#include <vector>
#include <queue>
#include <utility>
using namespace std;

const int INF = 1e9;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, E;
    cin >> V >> E;
    int start;
    cin >> start;

    vector<vector<pair<int, int>>> graph(V + 1);
    vector<int> distance(V + 1, INF);

    for (int i = 0; i < E; ++i) {
        int a, b, cost;
        cin >> a >> b >> cost;
        graph[a].push_back({b, cost});
    }

    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> queue;
    queue.push({0, start});
    distance[start] = 0;

    while (!queue.empty()) {
        auto [dist, now] = queue.top();
        queue.pop();

        if (dist > distance[now]) continue;

        for (auto [next_node, next_dist] : graph[now]) {
            int cost = dist + next_dist;
            if (cost < distance[next_node]) {
                distance[next_node] = cost;
                queue.push({cost, next_node});
            }
        }
    }

    for (int i = 1; i <= V; ++i) {
        if (distance[i] == INF) cout << "INF\n";
        else cout << distance[i] << "\n";
    }
    return 0;
}
