#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> graph(n + 1);
    for (int i = 0; i < n - 1; ++i) {
        int a, b;
        cin >> a >> b;
        graph[a].push_back(b);
        graph[b].push_back(a);
    }

    vector<int> parents(n + 1, 0);

    queue<int> q;
    q.push(1);
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        for (int neighbor : graph[node]) {
            if (parents[neighbor] == 0 && neighbor != 1) {
                parents[neighbor] = node;
                q.push(neighbor);
            }
        }
    }

    for (int i = 2; i <= n; ++i) cout << parents[i] << "\n";
    return 0;
}
