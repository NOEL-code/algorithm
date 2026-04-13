#include <iostream>
#include <vector>
#include <array>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<vector<int>> graph(N, vector<int>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            cin >> graph[i][j];

    vector<vector<array<int, 3>>> dp(N, vector<array<int, 3>>(N, {0, 0, 0}));
    dp[0][1][0] = 1;

    for (int x = 0; x < N; ++x) {
        for (int y = 2; y < N; ++y) {
            if (graph[x][y] == 1) continue;

            if (graph[x][y - 1] == 0)
                dp[x][y][0] = dp[x][y - 1][0] + dp[x][y - 1][2];

            if (x > 0 && graph[x - 1][y] == 0)
                dp[x][y][1] = dp[x - 1][y][1] + dp[x - 1][y][2];

            if (x > 0 && graph[x][y - 1] == 0 && graph[x - 1][y] == 0 && graph[x - 1][y - 1] == 0)
                dp[x][y][2] = dp[x - 1][y - 1][0] + dp[x - 1][y - 1][1] + dp[x - 1][y - 1][2];
        }
    }

    cout << dp[N - 1][N - 1][0] + dp[N - 1][N - 1][1] + dp[N - 1][N - 1][2] << "\n";
    return 0;
}
