#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int N, M;
vector<vector<int>> grid;

void bfs(int sx, int sy, vector<vector<bool>>& visited) {
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, -1, 0, 1};
    queue<pair<int, int>> q;
    q.push({sx, sy});
    visited[sx][sy] = true;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx < 0 || nx >= N || ny < 0 || ny >= M) continue;
            if (!visited[nx][ny] && grid[nx][ny] > 0) {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }
}

void melt() {
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, -1, 0, 1};
    vector<vector<int>> minus(N, vector<int>(M, 0));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (grid[i][j] > 0) {
                int sea_cnt = 0;
                for (int k = 0; k < 4; ++k) {
                    int ni = i + dx[k];
                    int nj = j + dy[k];
                    if (ni < 0 || ni >= N || nj < 0 || nj >= M) continue;
                    if (grid[ni][nj] == 0) ++sea_cnt;
                }
                minus[i][j] = sea_cnt;
            }
        }
    }
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (grid[i][j] > 0) {
                grid[i][j] -= minus[i][j];
                if (grid[i][j] < 0) grid[i][j] = 0;
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    grid.assign(N, vector<int>(M));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            cin >> grid[i][j];

    int year = 0;
    while (true) {
        vector<vector<bool>> visited(N, vector<bool>(M, false));
        int count = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                if (grid[i][j] > 0 && !visited[i][j]) {
                    bfs(i, j, visited);
                    ++count;
                }
            }
        }

        if (count >= 2) {
            cout << year << "\n";
            break;
        }
        if (count == 0) {
            cout << 0 << "\n";
            break;
        }

        melt();
        ++year;
    }
    return 0;
}
