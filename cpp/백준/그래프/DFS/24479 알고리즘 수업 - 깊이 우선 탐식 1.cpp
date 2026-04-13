#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int N, M, R;
vector<vector<int>> graph;
vector<int> visited;
int order = 1;

void dfs(int start) {
    for (int neighbor : graph[start]) {
        if (visited[neighbor] == 0) {
            order += 1;
            visited[neighbor] = order;
            dfs(neighbor);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M >> R;
    graph.assign(N + 1, {});
    visited.assign(N + 1, 0);

    for (int i = 0; i < M; ++i) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    for (int i = 1; i <= N; ++i) sort(graph[i].begin(), graph[i].end());

    visited[R] = order;
    dfs(R);

    for (int i = 1; i <= N; ++i) cout << visited[i] << "\n";
    return 0;
}
