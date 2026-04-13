#include <iostream>
#include <vector>
#include <algorithm>
#include <tuple>
using namespace std;

int N, M;
vector<vector<int>> board;
vector<tuple<int, int, int>> cctv;

vector<vector<vector<int>>> types = {
    {},
    {{0}, {1}, {2}, {3}},
    {{0, 2}, {1, 3}},
    {{0, 1}, {1, 2}, {2, 3}, {3, 0}},
    {{0, 1, 2}, {1, 2, 3}, {2, 3, 0}, {3, 0, 1}},
    {{0, 1, 2, 3}}
};

int dx[] = {-1, 0, 1, 0};
int dy[] = {0, 1, 0, -1};

void watch(vector<vector<int>>& b, int r, int c, int d) {
    while (true) {
        r += dx[d];
        c += dy[d];
        if (r < 0 || r >= N || c < 0 || c >= M) break;
        if (b[r][c] == 6) break;
        if (b[r][c] == 0) b[r][c] = -1;
    }
}

int answer;

void solve(int idx, vector<vector<int>>& b) {
    if (idx == (int)cctv.size()) {
        int count = 0;
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < M; ++j)
                if (b[i][j] == 0) count++;
        answer = min(answer, count);
        return;
    }

    int r, c, t;
    tie(r, c, t) = cctv[idx];

    for (int choice = 0; choice < (int)types[t].size(); ++choice) {
        vector<vector<int>> copy = b;
        for (int d : types[t][choice]) {
            watch(copy, r, c, d);
        }
        solve(idx + 1, copy);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    board.assign(N, vector<int>(M));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            cin >> board[i][j];
            if (board[i][j] >= 1 && board[i][j] <= 5) {
                cctv.push_back({i, j, board[i][j]});
            }
        }
    }

    answer = N * M;
    solve(0, board);
    cout << answer << "\n";
    return 0;
}
