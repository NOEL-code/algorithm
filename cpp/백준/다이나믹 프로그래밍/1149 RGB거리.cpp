#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<vector<int>> graph(N, vector<int>(3));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < 3; ++j)
            cin >> graph[i][j];

    vector<vector<int>> dp(N, vector<int>(3, 0));
    dp[0] = graph[0];

    for (int i = 1; i < N; ++i) {
        dp[i][0] = min(dp[i - 1][1], dp[i - 1][2]) + graph[i][0];
        dp[i][1] = min(dp[i - 1][0], dp[i - 1][2]) + graph[i][1];
        dp[i][2] = min(dp[i - 1][0], dp[i - 1][1]) + graph[i][2];
    }

    cout << min({dp[N - 1][0], dp[N - 1][1], dp[N - 1][2]}) << "\n";
    return 0;
}
