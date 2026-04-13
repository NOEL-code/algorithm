#include <iostream>
#include <vector>
using namespace std;

int n, m;
vector<vector<int>> graph;
vector<bool> visited;

bool check_cycle(int node, int parent) {
    for (int neighbor : graph[node]) {
        if (neighbor == parent) continue;
        if (visited[neighbor]) return true;
        visited[neighbor] = true;
        if (check_cycle(neighbor, node)) return true;
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int caseNum = 1;
    while (true) {
        cin >> n >> m;
        if (n == 0 && m == 0) break;

        graph.assign(n + 1, {});
        for (int i = 0; i < m; ++i) {
            int a, b;
            cin >> a >> b;
            graph[a].push_back(b);
            graph[b].push_back(a);
        }

        visited.assign(n + 1, false);

        int tree_count = 0;
        for (int i = 1; i <= n; ++i) {
            if (!visited[i]) {
                visited[i] = true;
                if (!check_cycle(i, 0)) tree_count += 1;
            }
        }

        if (tree_count == 0) {
            cout << "Case " << caseNum << ": No trees.\n";
        } else if (tree_count == 1) {
            cout << "Case " << caseNum << ": There is one tree.\n";
        } else {
            cout << "Case " << caseNum << ": A forest of " << tree_count << " trees.\n";
        }
        caseNum += 1;
    }
    return 0;
}
