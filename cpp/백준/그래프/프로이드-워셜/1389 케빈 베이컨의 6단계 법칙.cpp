#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

const int INF = 1e9;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<int>> graph(N + 1, vector<int>(N + 1, INF));
    for (int a = 1; a <= N; ++a) graph[a][a] = 0;

    for (int i = 0; i < M; ++i) {
        int a, b;
        cin >> a >> b;
        graph[a][b] = 1;
        graph[b][a] = 1;
    }

    for (int k = 1; k <= N; ++k) {
        for (int a = 1; a <= N; ++a) {
            for (int b = 1; b <= N; ++b) {
                graph[a][b] = min(graph[a][b], graph[a][k] + graph[k][b]);
            }
        }
    }

    vector<int> count_array(N + 1, 0);
    for (int a = 1; a <= N; ++a) {
        for (int b = 1; b <= N; ++b) {
            if (graph[a][b] != INF) count_array[a] += graph[a][b];
        }
    }

    int min_value = count_array[1];
    int min_index = 1;
    for (int i = 2; i <= N; ++i) {
        if (count_array[i] < min_value) {
            min_value = count_array[i];
            min_index = i;
        }
    }

    cout << min_index << "\n";
    return 0;
}
