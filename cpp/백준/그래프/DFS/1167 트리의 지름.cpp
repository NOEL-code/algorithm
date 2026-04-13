#include <iostream>
#include <vector>
#include <utility>
using namespace std;

int N;
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

    cin >> N;
    graph.assign(N + 1, {});

    for (int i = 0; i < N; ++i) {
        int u;
        cin >> u;
        while (true) {
            int v, c;
            cin >> v;
            if (v == -1) break;
            cin >> c;
            graph[u].push_back({v, c});
        }
    }

    visited.assign(N + 1, -1);
    visited[1] = 0;
    dfs(1, 0);

    int max_dist = 0, max_node = 1;
    for (int i = 1; i <= N; ++i) {
        if (visited[i] > max_dist) {
            max_dist = visited[i];
            max_node = i;
        }
    }

    visited.assign(N + 1, -1);
    visited[max_node] = 0;
    dfs(max_node, 0);

    int ans = 0;
    for (int i = 1; i <= N; ++i) ans = max(ans, visited[i]);
    cout << ans << "\n";
    return 0;
}
