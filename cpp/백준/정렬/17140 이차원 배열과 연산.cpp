#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, C, K;
    cin >> R >> C >> K;

    vector<vector<int>> A(100, vector<int>(100, 0));

    int row_count = 3;
    int col_count = 3;

    for (int r = 0; r < 3; ++r) {
        for (int c = 0; c < 3; ++c) {
            cin >> A[r][c];
        }
    }

    for (int t = 0; t <= 100; ++t) {
        if (R <= row_count && C <= col_count && A[R - 1][C - 1] == K) {
            cout << t << "\n";
            return 0;
        }

        if (t == 100) break;

        if (row_count >= col_count) {
            int new_col = 0;
            for (int r = 0; r < row_count; ++r) {
                vector<int> count(101, 0);
                for (int c = 0; c < col_count; ++c) {
                    if (A[r][c] != 0) count[A[r][c]] += 1;
                }

                vector<pair<int, int>> pairs;
                for (int num = 1; num <= 100; ++num) {
                    if (count[num] > 0) pairs.push_back({count[num], num});
                }
                sort(pairs.begin(), pairs.end());

                A[r] = vector<int>(100, 0);

                int idx = 0;
                for (auto& p : pairs) {
                    if (idx >= 100) break;
                    A[r][idx] = p.second;
                    A[r][idx + 1] = p.first;
                    idx += 2;
                }
                new_col = max(new_col, idx);
            }
            col_count = min(new_col, 100);
        } else {
            int new_row = 0;
            for (int c = 0; c < col_count; ++c) {
                vector<int> count(101, 0);
                for (int r = 0; r < row_count; ++r) {
                    if (A[r][c] != 0) count[A[r][c]] += 1;
                }

                vector<pair<int, int>> pairs;
                for (int num = 1; num <= 100; ++num) {
                    if (count[num] > 0) pairs.push_back({count[num], num});
                }
                sort(pairs.begin(), pairs.end());

                for (int r = 0; r < 100; ++r) A[r][c] = 0;

                int idx = 0;
                for (auto& p : pairs) {
                    if (idx >= 100) break;
                    A[idx][c] = p.second;
                    A[idx + 1][c] = p.first;
                    idx += 2;
                }
                new_row = max(new_row, idx);
            }
            row_count = min(new_row, 100);
        }
    }

    cout << -1 << "\n";
    return 0;
}
