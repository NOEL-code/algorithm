#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int n, m;
vector<vector<int>> grid;
vector<vector<int>> board;

void clear_board() {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            board[i][j] = 0;
}

void draw(int x1, int y1, int x2, int y2) {
    for (int i = x1; i <= x2; ++i)
        for (int j = y1; j <= y2; ++j)
            board[i][j] += 1;
}

bool check_board() {
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            if (board[i][j] >= 2) return true;
    return false;
}

bool overlapped(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
    clear_board();
    draw(x1, y1, x2, y2);
    draw(x3, y3, x4, y4);
    return check_board();
}

int rect_sum(int x1, int y1, int x2, int y2) {
    int s = 0;
    for (int i = x1; i <= x2; ++i)
        for (int j = y1; j <= y2; ++j)
            s += grid[i][j];
    return s;
}

int find_max_sum_with_rect(int x1, int y1, int x2, int y2) {
    int max_sum = INT_MIN;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            for (int k = i; k < n; ++k) {
                for (int l = j; l < m; ++l) {
                    if (!overlapped(x1, y1, x2, y2, i, j, k, l)) {
                        max_sum = max(max_sum, rect_sum(x1, y1, x2, y2) + rect_sum(i, j, k, l));
                    }
                }
            }
        }
    }

    return max_sum;
}

int find_max_sum() {
    int max_sum = INT_MIN;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            for (int k = i; k < n; ++k) {
                for (int l = j; l < m; ++l) {
                    max_sum = max(max_sum, find_max_sum_with_rect(i, j, k, l));
                }
            }
        }
    }
    return max_sum;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n >> m;

    grid.assign(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < m; ++j)
            cin >> grid[i][j];

    board.assign(n, vector<int>(m, 0));

    cout << find_max_sum() << "\n";
    return 0;
}
