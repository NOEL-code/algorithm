#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<long long> dp(n + 1, 0);
    dp[1] = 1;
    if (n >= 2) dp[2] = 1;

    for (int i = 3; i <= n; ++i) {
        dp[i] = dp[i - 1] + dp[i - 2];
    }

    cout << dp[n] << ' ' << n - 2 << "\n";
    return 0;
}
