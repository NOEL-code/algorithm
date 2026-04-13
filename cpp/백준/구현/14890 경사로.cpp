#include <iostream>
#include <vector>
using namespace std;

int N, L;
vector<vector<int>> board;

bool check(const vector<int>& line) {
    vector<bool> used(N, false);
    for (int i = 0; i < N - 1; ++i) {
        int diff = line[i + 1] - line[i];
        if (diff == 0) continue;
        else if (diff == 1) {
            for (int j = i; j > i - L; --j) {
                if (j < 0 || line[j] != line[i] || used[j]) return false;
                used[j] = true;
            }
        } else if (diff == -1) {
            for (int j = i + 1; j < i + 1 + L; ++j) {
                if (j >= N || line[j] != line[i + 1] || used[j]) return false;
                used[j] = true;
            }
        } else {
            return false;
        }
    }
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> L;

    board.assign(N, vector<int>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            cin >> board[i][j];

    int result = 0;

    for (int i = 0; i < N; ++i) {
        if (check(board[i])) result++;
        vector<int> col(N);
        for (int r = 0; r < N; ++r) col[r] = board[r][i];
        if (check(col)) result++;
    }

    cout << result << "\n";
    return 0;
}
