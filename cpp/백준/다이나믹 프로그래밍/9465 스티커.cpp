#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int n;
        cin >> n;

        vector<vector<int>> graph(2, vector<int>(n));
        for (int i = 0; i < 2; ++i)
            for (int j = 0; j < n; ++j)
                cin >> graph[i][j];

        vector<vector<int>> dp(2, vector<int>(n, 0));

        dp[0][0] = graph[0][0];
        dp[1][0] = graph[1][0];

        if (n == 1) {
            cout << max(graph[0][0], graph[1][0]) << "\n";
            continue;
        }

        dp[0][1] = graph[1][0] + graph[0][1];
        dp[1][1] = graph[0][0] + graph[1][1];

        for (int i = 2; i < n; ++i) {
            dp[0][i] = max(dp[1][i - 2], dp[1][i - 1]) + graph[0][i];
            dp[1][i] = max(dp[0][i - 2], dp[0][i - 1]) + graph[1][i];
        }

        cout << max(dp[0][n - 1], dp[1][n - 1]) << "\n";
    }

    return 0;
}
