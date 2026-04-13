#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
using namespace std;

int N, M;
vector<vector<int>> grid;

int BFS(vector<vector<int>> temp) {
    int dx[] = {0, -1, 1, 0};
    int dy[] = {1, 0, 0, -1};
    queue<pair<int, int>> q;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (temp[i][j] == 2) q.push({i, j});

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx < 0 || nx >= N || ny < 0 || ny >= M) continue;
            if (temp[nx][ny] == 0) {
                temp[nx][ny] = 2;
                q.push({nx, ny});
            }
        }
    }

    int safe = 0;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (temp[i][j] == 0) ++safe;
    return safe;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    grid.assign(N, vector<int>(M));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            cin >> grid[i][j];

    vector<pair<int, int>> empty;
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            if (grid[i][j] == 0) empty.push_back({i, j});

    int ans = 0;
    int E = (int)empty.size();
    for (int a = 0; a < E; ++a) {
        for (int b = a + 1; b < E; ++b) {
            for (int c = b + 1; c < E; ++c) {
                auto temp = grid;
                temp[empty[a].first][empty[a].second] = 1;
                temp[empty[b].first][empty[b].second] = 1;
                temp[empty[c].first][empty[c].second] = 1;
                ans = max(ans, BFS(temp));
            }
        }
    }

    cout << ans << "\n";
    return 0;
}
