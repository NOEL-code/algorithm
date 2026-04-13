#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <set>
#include <algorithm>
using namespace std;

int R, C;
vector<string> grid;

void throwMineral(int turn, int height) {
    int row = R - height;
    if (turn % 2 == 0) {
        for (int c = 0; c < C; ++c) {
            if (grid[row][c] == 'x') {
                grid[row][c] = '.';
                return;
            }
        }
    } else {
        for (int c = C - 1; c >= 0; --c) {
            if (grid[row][c] == 'x') {
                grid[row][c] = '.';
                return;
            }
        }
    }
}

vector<pair<int, int>> find_floating() {
    int dx[] = {0, 1, -1, 0};
    int dy[] = {1, 0, 0, -1};
    vector<vector<bool>> visited(R, vector<bool>(C, false));
    queue<pair<int, int>> q;
    for (int c = 0; c < C; ++c) {
        if (grid[R - 1][c] == 'x') {
            visited[R - 1][c] = true;
            q.push({R - 1, c});
        }
    }
    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        for (int i = 0; i < 4; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx < 0 || nx >= R || ny < 0 || ny >= C) continue;
            if (!visited[nx][ny] && grid[nx][ny] == 'x') {
                visited[nx][ny] = true;
                q.push({nx, ny});
            }
        }
    }
    vector<pair<int, int>> floating;
    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            if (grid[r][c] == 'x' && !visited[r][c]) floating.push_back({r, c});
        }
    }
    return floating;
}

void drop(vector<pair<int, int>>& floating) {
    set<pair<int, int>> floating_set(floating.begin(), floating.end());
    int move = R;
    for (auto [r, c] : floating) {
        int dist = 0;
        int nr = r + 1;
        while (nr < R && floating_set.find({nr, c}) == floating_set.end() && grid[nr][c] != 'x') {
            ++dist;
            ++nr;
        }
        move = min(move, dist);
    }
    if (move == 0) return;
    for (auto [r, c] : floating) grid[r][c] = '.';
    for (auto [r, c] : floating) grid[r + move][c] = 'x';
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> R >> C;
    grid.resize(R);
    for (int i = 0; i < R; ++i) cin >> grid[i];

    int N;
    cin >> N;
    vector<int> H(N);
    for (int i = 0; i < N; ++i) cin >> H[i];

    for (int turn = 0; turn < N; ++turn) {
        throwMineral(turn, H[turn]);
        auto floating = find_floating();
        if (!floating.empty()) drop(floating);
    }

    for (int r = 0; r < R; ++r) cout << grid[r] << "\n";
    return 0;
}
