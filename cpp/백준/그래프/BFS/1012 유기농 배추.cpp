#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int M, N, K;

void bfs(int x, int y, vector<vector<int>>& graph, vector<vector<bool>>& visited) {
    int dx[] = {-1, 0, 1, 0};
    int dy[] = {0, -1, 0, 1};
    queue<pair<int, int>> q;
    q.push({x, y});
    visited[y][x] = true;

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (nx < 0 || nx >= M || ny < 0 || ny >= N) continue;
            if (!visited[ny][nx] && graph[ny][nx] == 1) {
                visited[ny][nx] = true;
                q.push({nx, ny});
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        cin >> M >> N >> K;
        vector<vector<int>> graph(N, vector<int>(M, 0));
        vector<vector<bool>> visited(N, vector<bool>(M, false));

        for (int i = 0; i < K; ++i) {
            int x, y;
            cin >> x >> y;
            graph[y][x] = 1;
        }

        int count = 0;
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < M; ++j) {
                if (graph[i][j] == 1 && !visited[i][j]) {
                    bfs(j, i, graph, visited);
                    ++count;
                }
            }
        }
        cout << count << "\n";
    }
    return 0;
}
