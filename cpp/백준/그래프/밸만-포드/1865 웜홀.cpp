#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int TC;
    cin >> TC;
    while (TC--) {
        int N, M, W;
        cin >> N >> M >> W;

        vector<tuple<int, int, int>> edges;

        for (int i = 0; i < M; ++i) {
            int S, E, T;
            cin >> S >> E >> T;
            edges.push_back({S, E, T});
            edges.push_back({E, S, T});
        }

        for (int i = 0; i < W; ++i) {
            int S, E, T;
            cin >> S >> E >> T;
            edges.push_back({S, E, -T});
        }

        vector<long long> dist(N + 1, 0);
        bool has_cycle = false;

        for (int i = 0; i < N; ++i) {
            for (auto [u, v, w] : edges) {
                if (dist[v] > dist[u] + w) {
                    dist[v] = dist[u] + w;
                    if (i == N - 1) {
                        has_cycle = true;
                        break;
                    }
                }
            }
            if (has_cycle) break;
        }

        cout << (has_cycle ? "YES" : "NO") << "\n";
    }
    return 0;
}
