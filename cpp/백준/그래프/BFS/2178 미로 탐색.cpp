#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<vector<int>> maze(N, vector<int>(M));
    for (int i = 0; i < N; ++i) {
        string row;
        cin >> row;
        for (int j = 0; j < M; ++j) maze[i][j] = row[j] - '0';
    }

    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    queue<pair<int, int>> q;
    q.push({0, 0});

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx < 0 || nx >= M || ny < 0 || ny >= N) continue;
            if (maze[ny][nx] == 1) {
                maze[ny][nx] = maze[y][x] + 1;
                q.push({nx, ny});
            }
        }
    }

    cout << maze[N - 1][M - 1] << "\n";
    return 0;
}
