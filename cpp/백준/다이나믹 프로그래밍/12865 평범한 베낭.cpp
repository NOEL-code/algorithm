#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, W;
    cin >> N >> W;

    vector<int> value;
    vector<int> weight;
    vector<vector<int>> dp(N + 1, vector<int>(W + 1, 0));

    for (int i = 0; i < N; ++i) {
        int w, v;
        cin >> w >> v;
        weight.push_back(w);
        value.push_back(v);
    }

    for (int i = 1; i <= N; ++i) {
        for (int w = 1; w <= W; ++w) {
            if (weight[i - 1] > w) {
                dp[i][w] = dp[i - 1][w];
            } else {
                dp[i][w] = max(dp[i - 1][w], value[i - 1] + dp[i - 1][w - weight[i - 1]]);
            }
        }
    }

    cout << dp[N][W] << "\n";
    return 0;
}
