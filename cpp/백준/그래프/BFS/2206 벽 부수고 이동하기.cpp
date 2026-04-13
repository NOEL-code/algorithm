#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <tuple>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<int>> grid(N, vector<int>(M));
    for (int i = 0; i < N; ++i) {
        string row;
        cin >> row;
        for (int j = 0; j < M; ++j) grid[i][j] = row[j] - '0';
    }

    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    vector<vector<vector<int>>> visited(N, vector<vector<int>>(M, vector<int>(2, 0)));
    queue<tuple<int, int, int>> q;
    q.push({0, 0, 0});
    visited[0][0][0] = 1;

    int ans = -1;
    while (!q.empty()) {
        auto [x, y, broken] = q.front();
        q.pop();

        if (x == N - 1 && y == M - 1) {
            ans = visited[x][y][broken];
            break;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx < 0 || nx >= N || ny < 0 || ny >= M) continue;
            if (grid[nx][ny] == 0 && visited[nx][ny][broken] == 0) {
                visited[nx][ny][broken] = visited[x][y][broken] + 1;
                q.push({nx, ny, broken});
            } else if (grid[nx][ny] == 1 && broken == 0 && visited[nx][ny][1] == 0) {
                visited[nx][ny][1] = visited[x][y][broken] + 1;
                q.push({nx, ny, 1});
            }
        }
    }

    cout << ans << "\n";
    return 0;
}
