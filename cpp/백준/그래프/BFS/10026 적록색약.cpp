#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

int N;
vector<string> graph;
vector<vector<bool>> visited;

void bfs(int x, int y, char color, bool weak) {
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};
    queue<pair<int, int>> q;
    q.push({x, y});
    visited[x][y] = true;

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();

        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
            if (visited[nx][ny]) continue;
            if (weak && (color == 'G' || color == 'R')) {
                if (graph[nx][ny] == 'G' || graph[nx][ny] == 'R') {
                    visited[nx][ny] = true;
                    q.push({nx, ny});
                }
            } else if (graph[nx][ny] == color) {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;
    graph.resize(N);
    for (int i = 0; i < N; ++i) cin >> graph[i];

    int count = 0;
    visited.assign(N, vector<bool>(N, false));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (!visited[i][j]) {
                bfs(i, j, graph[i][j], false);
                ++count;
            }
        }
    }

    int color_weakness_count = 0;
    visited.assign(N, vector<bool>(N, false));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (!visited[i][j]) {
                bfs(i, j, graph[i][j], true);
                ++color_weakness_count;
            }
        }
    }

    cout << count << ' ' << color_weakness_count << "\n";
    return 0;
}
