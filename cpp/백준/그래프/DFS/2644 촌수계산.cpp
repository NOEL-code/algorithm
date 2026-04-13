#include <iostream>
#include <vector>
using namespace std;

int n, start, endNode;
vector<vector<int>> graph;
vector<int> visited;

int dfs(int node, int dist) {
    if (node == endNode) return dist;
    for (int next_node : graph[node]) {
        if (visited[next_node] == -1) {
            visited[next_node] = dist + 1;
            int result = dfs(next_node, dist + 1);
            if (result != -1) return result;
        }
    }
    return -1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    cin >> start >> endNode;

    graph.assign(n + 1, {});

    int m;
    cin >> m;

    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    visited.assign(n + 1, -1);
    visited[start] = 0;
    cout << dfs(start, 0) << "\n";
    return 0;
}
