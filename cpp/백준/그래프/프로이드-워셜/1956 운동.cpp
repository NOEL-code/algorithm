#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const long long INF = 1e15;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int V, E;
    cin >> V >> E;

    vector<vector<long long>> graph(V + 1, vector<long long>(V + 1, INF));

    for (int i = 0; i < E; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        graph[a][b] = c;
    }

    for (int k = 1; k <= V; ++k) {
        for (int a = 1; a <= V; ++a) {
            for (int b = 1; b <= V; ++b) {
                graph[a][b] = min(graph[a][b], graph[a][k] + graph[k][b]);
            }
        }
    }

    long long answer = INF;
    for (int i = 1; i <= V; ++i) {
        for (int j = 1; j <= V; ++j) {
            if (i == j) continue;
            answer = min(answer, graph[i][j] + graph[j][i]);
        }
    }

    if (answer >= INF) cout << -1 << "\n";
    else cout << answer << "\n";
    return 0;
}
