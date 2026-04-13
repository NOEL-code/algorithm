#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int INF = 1e9;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n;
    cin >> m;

    vector<vector<int>> graph(n + 1, vector<int>(n + 1, INF));
    for (int a = 1; a <= n; ++a) graph[a][a] = 0;

    for (int i = 0; i < m; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        graph[a][b] = min(graph[a][b], c);
    }

    for (int k = 1; k <= n; ++k) {
        for (int a = 1; a <= n; ++a) {
            for (int b = 1; b <= n; ++b) {
                graph[a][b] = min(graph[a][b], graph[a][k] + graph[k][b]);
            }
        }
    }

    for (int a = 1; a <= n; ++a) {
        for (int b = 1; b <= n; ++b) {
            if (graph[a][b] == INF) cout << 0;
            else cout << graph[a][b];
            if (b < n) cout << " ";
        }
        cout << "\n";
    }
    return 0;
}
