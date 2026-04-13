#include <iostream>
#include <vector>
#include <tuple>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, K;
    cin >> N >> M >> K;

    int directions[8][2] = {{-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, -1}, {-1, -1}};

    vector<tuple<int, int, int, int, int>> fireballs;
    for (int i = 0; i < M; ++i) {
        int r, c, m, s, d;
        cin >> r >> c >> m >> s >> d;
        fireballs.push_back({r - 1, c - 1, m, s, d});
    }

    for (int k = 0; k < K; ++k) {
        vector<vector<vector<tuple<int, int, int>>>> grid(N, vector<vector<tuple<int, int, int>>>(N));
        for (auto& [r, c, m, s, d] : fireballs) {
            int dr = directions[d][0];
            int dc = directions[d][1];
            int nr = ((r + dr * s) % N + N) % N;
            int nc = ((c + dc * s) % N + N) % N;
            grid[nr][nc].push_back({m, s, d});
        }

        fireballs.clear();
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                if (grid[r][c].size() == 1) {
                    auto [m, s, d] = grid[r][c][0];
                    fireballs.push_back({r, c, m, s, d});
                } else if (grid[r][c].size() >= 2) {
                    int sum_m = 0, sum_s = 0;
                    int odd = 0, even = 0;

                    for (auto& [m, s, d] : grid[r][c]) {
                        if (d % 2 == 0) even++;
                        else odd++;
                        sum_m += m;
                        sum_s += s;
                    }

                    bool even_flag = false;
                    if (odd == 0 || even == 0) even_flag = true;

                    int new_s = sum_s / (int)grid[r][c].size();
                    int new_m = sum_m / 5;

                    if (new_m == 0) continue;

                    if (even_flag) {
                        for (int i = 0; i < 8; i += 2)
                            fireballs.push_back({r, c, new_m, new_s, i});
                    } else {
                        for (int i = 1; i < 8; i += 2)
                            fireballs.push_back({r, c, new_m, new_s, i});
                    }
                }
            }
        }
    }

    int total = 0;
    for (auto& [r, c, m, s, d] : fireballs) total += m;
    cout << total << "\n";
    return 0;
}
