#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int M, N;
    cin >> M >> N;

    vector<vector<int>> graph(N, vector<int>(M));
    queue<pair<int, int>> q;

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cin >> graph[i][j];
            if (graph[i][j] == 1) q.push({j, i});
        }
    }

    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx < 0 || nx >= M || ny < 0 || ny >= N) continue;
            if (graph[ny][nx] == 0) {
                graph[ny][nx] = graph[y][x] + 1;
                q.push({nx, ny});
            }
        }
    }

    int maxDays = 0;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (graph[i][j] == 0) {
                cout << -1 << "\n";
                return 0;
            }
            maxDays = max(maxDays, graph[i][j]);
        }
    }

    cout << maxDays - 1 << "\n";
    return 0;
}
