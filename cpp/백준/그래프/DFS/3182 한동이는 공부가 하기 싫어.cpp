#include <iostream>
#include <vector>
using namespace std;

int n;
vector<int> graph;
vector<bool> visited;

void dfs(int start) {
    visited[start] = true;
    if (!visited[graph[start]]) {
        dfs(graph[start]);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    graph.assign(n + 1, 0);

    for (int i = 1; i <= n; ++i) cin >> graph[i];

    int best_index = 0;
    int best_count = 0;

    for (int i = 1; i <= n; ++i) {
        visited.assign(n + 1, false);
        dfs(i);
        int cnt = 0;
        for (int j = 1; j <= n; ++j) if (visited[j]) cnt++;
        if (cnt > best_count) {
            best_count = cnt;
            best_index = i;
        }
    }

    cout << best_index << "\n";
    return 0;
}
