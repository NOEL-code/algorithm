#include <iostream>
#include <vector>
#include <queue>
#include <utility>
#include <algorithm>
using namespace std;

const int INF = 1e9;

vector<int> dijkstra(int start, vector<vector<pair<int, int>>>& graph, int N) {
    vector<int> distance(N + 1, INF);
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
    return distance;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, X;
    cin >> N >> M >> X;

    vector<vector<pair<int, int>>> graph(N + 1);
    vector<vector<pair<int, int>>> reverse_graph(N + 1);

    for (int i = 0; i < M; ++i) {
        int a, b, cost;
        cin >> a >> b >> cost;
        graph[a].push_back({b, cost});
        reverse_graph[b].push_back({a, cost});
    }

    vector<int> dist_from_X = dijkstra(X, graph, N);
    vector<int> dist_to_X = dijkstra(X, reverse_graph, N);

    int ans = 0;
    for (int i = 1; i <= N; ++i) {
        ans = max(ans, dist_to_X[i] + dist_from_X[i]);
    }

    cout << ans << "\n";
    return 0;
}
