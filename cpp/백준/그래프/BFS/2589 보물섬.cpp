#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
using namespace std;

int N, M;
vector<string> treasure_map;

int BFS(int sy, int sx) {
    int dy[] = {0, 1, -1, 0};
    int dx[] = {1, 0, 0, -1};
    vector<vector<int>> visited(N, vector<int>(M, -1));
    queue<pair<int, int>> q;
    q.push({sy, sx});
    visited[sy][sx] = 0;
    int max_dist = 0;

    while (!q.empty()) {
        auto [y, x] = q.front();
        q.pop();
        for (int i = 0; i < 4; ++i) {
            int ny = y + dy[i];
            int nx = x + dx[i];
            if (ny < 0 || ny >= N || nx < 0 || nx >= M) continue;
            if (visited[ny][nx] == -1 && treasure_map[ny][nx] == 'L') {
                visited[ny][nx] = visited[y][x] + 1;
                max_dist = max(max_dist, visited[ny][nx]);
                q.push({ny, nx});
            }
        }
    }
    return max_dist;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    treasure_map.resize(N);
    for (int i = 0; i < N; ++i) cin >> treasure_map[i];

    int ans = 0;
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            if (treasure_map[j][i] == 'L') {
                int dist = BFS(j, i);
                ans = max(ans, dist);
            }
        }
    }

    cout << ans << "\n";
    return 0;
}
