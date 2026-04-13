#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int n;
vector<vector<int>> grid;
vector<vector<long long>> pref1;
vector<vector<long long>> pref2;

long long sum_main(int r1, int c1, int r2, int c2) {
    return pref1[r2 + 1][c2 + 1] - pref1[r1][c1];
}

long long sum_anti(int r1, int c1, int r2, int c2) {
    return pref2[r2 + 1][c2] - pref2[r1][c1 + 1];
}

bool inb(int r, int c) {
    return r >= 0 && r < n && c >= 0 && c < n;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    grid.assign(n, vector<int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            cin >> grid[i][j];

    pref1.assign(n + 1, vector<long long>(n + 1, 0));
    for (int r = 0; r < n; ++r)
        for (int c = 0; c < n; ++c)
            pref1[r + 1][c + 1] = grid[r][c] + pref1[r][c];

    pref2.assign(n + 1, vector<long long>(n + 1, 0));
    for (int r = 0; r < n; ++r)
        for (int c = n - 1; c >= 0; --c)
            pref2[r + 1][c] = grid[r][c] + pref2[r][c + 1];

    long long max_sum = LLONG_MIN;

    for (int r = 0; r < n; ++r) {
        for (int c = 0; c < n; ++c) {
            int a = 1;
            while (inb(r + a, c + a)) {
                int b = 1;
                while (true) {
                    int rr = r + a, rc = c + a;
                    int br = r + a + b, bc = c + a - b;
                    int lr = r + b, lc = c - b;

                    if (!(inb(lr, lc) && inb(br, bc))) break;

                    long long s1 = sum_main(r, c, rr, rc);
                    long long s2 = sum_anti(rr, rc, br, bc);
                    long long s3 = sum_main(lr, lc, br, bc);
                    long long s4 = sum_anti(r, c, lr, lc);

                    long long perimeter = s1 + s2 + s3 + s4
                        - (grid[r][c] + grid[rr][rc] + grid[br][bc] + grid[lr][lc]);

                    if (perimeter > max_sum) {
                        max_sum = perimeter;
                    }

                    b++;
                }
                a++;
            }
        }
    }

    cout << max_sum << "\n";
    return 0;
}
