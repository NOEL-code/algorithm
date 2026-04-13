#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

int N, M;
vector<string> graph;
vector<vector<bool>> visited;

int bfs(int x, int y) {
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};
    queue<pair<int, int>> q;
    q.push({x, y});
    visited[x][y] = true;
    int count = 0;

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();
        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (nx < 0 || nx >= N || ny < 0 || ny >= M) continue;
            if (!visited[nx][ny] && graph[nx][ny] != 'X') {
                visited[nx][ny] = true;
                q.push({nx, ny});
                if (graph[nx][ny] == 'P') ++count;
            }
        }
    }
    return count;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    graph.resize(N);
    for (int i = 0; i < N; ++i) cin >> graph[i];
    visited.assign(N, vector<bool>(M, false));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            if (graph[i][j] == 'I') {
                int count = bfs(i, j);
                if (count > 0) cout << count << "\n";
                else cout << "TT" << "\n";
            }
        }
    }
    return 0;
}
