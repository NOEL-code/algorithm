#include <iostream>
#include <vector>
#include <utility>
using namespace std;

int n;
vector<vector<pair<int, int>>> graph;
vector<int> visited;

void dfs(int node, int dist) {
    for (auto [next_node, cost] : graph[node]) {
        if (visited[next_node] == -1) {
            visited[next_node] = dist + cost;
            dfs(next_node, dist + cost);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    graph.assign(n + 1, {});

    for (int i = 0; i < n - 1; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        graph[a].push_back({b, c});
        graph[b].push_back({a, c});
    }

    visited.assign(n + 1, -1);
    visited[1] = 0;
    dfs(1, 0);

    int max_dist = 0, farthest_node = 1;
    for (int i = 1; i <= n; ++i) {
        if (visited[i] > max_dist) {
            max_dist = visited[i];
            farthest_node = i;
        }
    }

    visited.assign(n + 1, -1);
    visited[farthest_node] = 0;
    dfs(farthest_node, 0);

    int ans = 0;
    for (int i = 1; i <= n; ++i) ans = max(ans, visited[i]);
    cout << ans << "\n";
    return 0;
}
