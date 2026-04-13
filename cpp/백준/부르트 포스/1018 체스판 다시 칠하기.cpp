#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <climits>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    vector<string> board(n);
    for (int i = 0; i < n; ++i) cin >> board[i];

    int best = INT_MAX;

    for (int i = 0; i <= n - 8; ++i) {
        for (int j = 0; j <= m - 8; ++j) {
            int draw_white = 0;
            int draw_black = 0;

            for (int a = i; a < i + 8; ++a) {
                for (int b = j; b < j + 8; ++b) {
                    if ((a + b) % 2 == 0) {
                        if (board[a][b] != 'W') draw_white++;
                        else draw_black++;
                    } else {
                        if (board[a][b] != 'W') draw_black++;
                        else draw_white++;
                    }
                }
            }

            best = min(best, min(draw_black, draw_white));
        }
    }

    cout << best << "\n";
    return 0;
}
