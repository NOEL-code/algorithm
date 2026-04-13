#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<vector<int>> graph(n, vector<int>(m));
    int target_x = -1, target_y = -1;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cin >> graph[i][j];
            if (graph[i][j] == 2) {
                target_x = j;
                target_y = i;
            }
        }
    }

    vector<vector<int>> result(n, vector<int>(m, -1));
    vector<vector<bool>> visited(n, vector<bool>(m, false));

    if (target_x != -1 && target_y != -1) {
        int dx[] = {1, 0, -1, 0};
        int dy[] = {0, 1, 0, -1};
        queue<pair<int, int>> q;
        q.push({target_x, target_y});
        result[target_y][target_x] = 0;
        visited[target_y][target_x] = true;

        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();
            for (int i = 0; i < 4; ++i) {
                int nx = x + dx[i];
                int ny = y + dy[i];
                if (nx < 0 || nx >= m || ny < 0 || ny >= n) continue;
                if (!visited[ny][nx] && graph[ny][nx] == 1) {
                    visited[ny][nx] = true;
                    result[ny][nx] = result[y][x] + 1;
                    q.push({nx, ny});
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (graph[i][j] == 0) result[i][j] = 0;
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            cout << result[i][j];
            if (j + 1 < m) cout << ' ';
        }
        cout << "\n";
    }
    return 0;
}
