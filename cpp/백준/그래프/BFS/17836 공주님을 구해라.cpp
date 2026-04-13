#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, T;
    cin >> N >> M >> T;

    vector<vector<int>> grid(N, vector<int>(M));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            cin >> grid[i][j];

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    vector<vector<vector<int>>> visited(N, vector<vector<int>>(M, vector<int>(2, -1)));
    queue<tuple<int, int, int>> q;
    q.push({0, 0, 0});
    visited[0][0][0] = 0;

    while (!q.empty()) {
        auto [x, y, sword] = q.front();
        q.pop();
        int time = visited[x][y][sword];

        if (time > T) continue;

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx < 0 || nx >= N || ny < 0 || ny >= M) continue;

            if (sword == 1) {
                if (visited[nx][ny][1] == -1) {
                    visited[nx][ny][1] = time + 1;
                    q.push({nx, ny, 1});
                }
            } else {
                if (grid[nx][ny] != 1) {
                    int ns = sword;
                    if (grid[nx][ny] == 2) ns = 1;
                    if (visited[nx][ny][ns] == -1) {
                        visited[nx][ny][ns] = time + 1;
                        q.push({nx, ny, ns});
                    }
                }
            }
        }
    }

    int ans = 1000000000;
    if (visited[N - 1][M - 1][0] != -1) ans = min(ans, visited[N - 1][M - 1][0]);
    if (visited[N - 1][M - 1][1] != -1) ans = min(ans, visited[N - 1][M - 1][1]);

    if (ans <= T) cout << ans << "\n";
    else cout << "Fail" << "\n";
    return 0;
}
