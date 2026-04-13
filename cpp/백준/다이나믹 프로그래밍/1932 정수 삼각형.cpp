#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<vector<int>> graph(n);
    for (int i = 0; i < n; ++i) {
        graph[i].resize(i + 1);
        for (int j = 0; j <= i; ++j) cin >> graph[i][j];
    }

    vector<vector<int>> dp(n, vector<int>(n, 0));
    dp[0][0] = graph[0][0];

    for (int i = 1; i < n; ++i) {
        for (int j = 0; j <= i; ++j) {
            if (j == 0) {
                dp[i][j] = dp[i - 1][j] + graph[i][j];
            } else if (j == i) {
                dp[i][j] = dp[i - 1][j - 1] + graph[i][j];
            } else {
                dp[i][j] = max(dp[i - 1][j - 1], dp[i - 1][j]) + graph[i][j];
            }
        }
    }

    cout << *max_element(dp[n - 1].begin(), dp[n - 1].end()) << "\n";
    return 0;
}
