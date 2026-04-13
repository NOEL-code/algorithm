#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int N, M;
vector<vector<int>> board;
vector<vector<bool>> visited;
int ans = 0;
int dx[] = {0, 0, 1, -1};
int dy[] = {1, -1, 0, 0};

void dfs(int x, int y, int depth, int total) {
    if (depth == 4) {
        ans = max(ans, total);
        return;
    }
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < N && ny >= 0 && ny < M && !visited[nx][ny]) {
            visited[nx][ny] = true;
            dfs(nx, ny, depth + 1, total + board[nx][ny]);
            visited[nx][ny] = false;
        }
    }
}

void check_t(int x, int y) {
    vector<int> neighbors;
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < N && ny >= 0 && ny < M) {
            neighbors.push_back(board[nx][ny]);
        }
    }
    if ((int)neighbors.size() >= 3) {
        int sum = 0;
        for (int v : neighbors) sum += v;
        if ((int)neighbors.size() == 4) {
            int mn = *min_element(neighbors.begin(), neighbors.end());
            ans = max(ans, board[x][y] + sum - mn);
        } else {
            ans = max(ans, board[x][y] + sum);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    board.assign(N, vector<int>(M));
    visited.assign(N, vector<bool>(M, false));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            cin >> board[i][j];

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            visited[i][j] = true;
            dfs(i, j, 1, board[i][j]);
            visited[i][j] = false;
            check_t(i, j);
        }
    }

    cout << ans << "\n";
    return 0;
}
