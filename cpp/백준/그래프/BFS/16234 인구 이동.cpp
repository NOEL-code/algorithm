#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
using namespace std;

int N, L, R;
vector<vector<int>> population;

pair<vector<pair<int, int>>, int> bfs(int x, int y, vector<vector<bool>>& visited) {
    int dx[] = {0, 1, -1, 0};
    int dy[] = {1, 0, 0, -1};
    queue<pair<int, int>> q;
    q.push({x, y});
    visited[x][y] = true;
    vector<pair<int, int>> group;
    group.push_back({x, y});
    int total = population[x][y];

    while (!q.empty()) {
        auto [cx, cy] = q.front();
        q.pop();
        for (int i = 0; i < 4; ++i) {
            int nx = cx + dx[i];
            int ny = cy + dy[i];
            if (nx < 0 || nx >= N || ny < 0 || ny >= N) continue;
            if (visited[nx][ny]) continue;
            int diff = abs(population[cx][cy] - population[nx][ny]);
            if (L <= diff && diff <= R) {
                visited[nx][ny] = true;
                group.push_back({nx, ny});
                total += population[nx][ny];
                q.push({nx, ny});
            }
        }
    }
    return {group, total};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> L >> R;
    population.assign(N, vector<int>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            cin >> population[i][j];

    int answer = 0;
    while (true) {
        bool remain = false;
        vector<vector<bool>> visited(N, vector<bool>(N, false));

        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                if (!visited[i][j]) {
                    auto [group, total] = bfs(i, j, visited);
                    if ((int)group.size() > 1) {
                        remain = true;
                        int new_population = total / (int)group.size();
                        for (auto [x, y] : group) population[x][y] = new_population;
                    }
                }
            }
        }

        if (!remain) break;
        ++answer;
    }

    cout << answer << "\n";
    return 0;
}
