#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int N, M, V;
vector<vector<int>> graph;
vector<bool> visited;

void dfs(int start) {
    visited[start] = true;
    cout << start << ' ';
    for (int neighbor : graph[start]) {
        if (!visited[neighbor]) dfs(neighbor);
    }
}

void bfs(int start) {
    queue<int> q;
    q.push(start);
    visited[start] = true;

    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << ' ';
        for (int neighbor : graph[node]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                q.push(neighbor);
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M >> V;

    graph.assign(N + 1, {});
    visited.assign(N + 1, false);

    for (int i = 0; i < M; ++i) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    for (int i = 1; i <= N; ++i) sort(graph[i].begin(), graph[i].end());

    dfs(V);
    cout << "\n";
    visited.assign(N + 1, false);
    bfs(V);
    cout << "\n";
    return 0;
}
