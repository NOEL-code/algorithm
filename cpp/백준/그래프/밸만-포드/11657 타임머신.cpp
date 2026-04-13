#include <iostream>
#include <vector>
#include <tuple>
#include <climits>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<tuple<int, int, int>> edges;
    edges.reserve(M);
    for (int i = 0; i < M; ++i) {
        int A, B, C;
        cin >> A >> B >> C;
        edges.push_back({A, B, C});
    }

    const long long INF = 1e18;
    vector<long long> dist(N + 1, INF);
    dist[1] = 0;

    for (int i = 0; i < N - 1; ++i) {
        for (auto [u, v, w] : edges) {
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    for (auto [u, v, w] : edges) {
        if (dist[u] != INF && dist[u] + w < dist[v]) {
            cout << -1 << "\n";
            return 0;
        }
    }

    for (int i = 2; i <= N; ++i) {
        if (dist[i] == INF) cout << -1 << "\n";
        else cout << dist[i] << "\n";
    }
    return 0;
}
