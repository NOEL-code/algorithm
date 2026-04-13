#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int m, n, h;
    cin >> m >> n >> h;

    vector<vector<vector<int>>> graph(h, vector<vector<int>>(n, vector<int>(m)));
    queue<tuple<int, int, int>> q;

    for (int z = 0; z < h; ++z) {
        for (int y = 0; y < n; ++y) {
            for (int x = 0; x < m; ++x) {
                cin >> graph[z][y][x];
                if (graph[z][y][x] == 1) q.push({x, y, z});
            }
        }
    }

    int dx[] = {1, 0, -1, 0, 0, 0};
    int dy[] = {0, 1, 0, -1, 0, 0};
    int dz[] = {0, 0, 0, 0, 1, -1};

    while (!q.empty()) {
        auto [x, y, z] = q.front();
        q.pop();
        for (int i = 0; i < 6; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            int nz = z + dz[i];
            if (nx < 0 || nx >= m || ny < 0 || ny >= n || nz < 0 || nz >= h) continue;
            if (graph[nz][ny][nx] == 0) {
                graph[nz][ny][nx] = graph[z][y][x] + 1;
                q.push({nx, ny, nz});
            }
        }
    }

    int max_days = 0;
    for (int z = 0; z < h; ++z) {
        for (int y = 0; y < n; ++y) {
            for (int x = 0; x < m; ++x) {
                if (graph[z][y][x] == 0) {
                    cout << -1 << "\n";
                    return 0;
                }
                max_days = max(max_days, graph[z][y][x]);
            }
        }
    }

    cout << max_days - 1 << "\n";
    return 0;
}
