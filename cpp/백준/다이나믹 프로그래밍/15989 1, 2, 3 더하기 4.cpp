#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int n;
        cin >> n;

        vector<vector<int>> dp(n + 1, vector<int>(4, 0));
        dp[0][1] = 1;
        dp[0][2] = 1;
        dp[0][3] = 1;

        for (int i = 1; i <= n; ++i) {
            dp[i][1] = 1;
            for (int j = 2; j < 4; ++j) {
                if (i - j >= 0) {
                    dp[i][j] = dp[i - j][j] + dp[i][j - 1];
                } else {
                    dp[i][j] = dp[i][j - 1];
                }
            }
        }

        cout << dp[n][3] << "\n";
    }

    return 0;
}
