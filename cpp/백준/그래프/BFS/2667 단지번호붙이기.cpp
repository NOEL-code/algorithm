#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
using namespace std;

int N;
vector<vector<int>> graph;
vector<vector<bool>> visited;

int bfs(int start_y, int start_x) {
    int dy[] = {1, 0, -1, 0};
    int dx[] = {0, 1, 0, -1};
    queue<pair<int, int>> q;
    q.push({start_y, start_x});
    visited[start_y][start_x] = true;
    int count = 1;

    while (!q.empty()) {
        auto [y, x] = q.front();
        q.pop();
        for (int i = 0; i < 4; ++i) {
            int ny = y + dy[i];
            int nx = x + dx[i];
            if (ny < 0 || ny >= N || nx < 0 || nx >= N) continue;
            if (!visited[ny][nx] && graph[ny][nx] == 1) {
                visited[ny][nx] = true;
                q.push({ny, nx});
                ++count;
            }
        }
    }
    return count;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;
    graph.assign(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        string row;
        cin >> row;
        for (int j = 0; j < N; ++j) graph[i][j] = row[j] - '0';
    }
    visited.assign(N, vector<bool>(N, false));

    vector<int> result;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            if (graph[i][j] == 1 && !visited[i][j]) {
                result.push_back(bfs(i, j));
            }
        }
    }

    sort(result.begin(), result.end());
    cout << result.size() << "\n";
    for (int r : result) cout << r << "\n";
    return 0;
}
