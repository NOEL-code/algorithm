#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<int> L(N), J(N);
    for (int i = 0; i < N; ++i) cin >> L[i];
    for (int i = 0; i < N; ++i) cin >> J[i];

    const int MAX_LOSS = 99;
    vector<int> dp(MAX_LOSS + 1, 0);

    for (int i = 0; i < N; ++i) {
        int lose = L[i];
        int joy = J[i];
        for (int h = MAX_LOSS; h >= lose; --h) {
            dp[h] = max(dp[h], dp[h - lose] + joy);
        }
    }

    cout << *max_element(dp.begin(), dp.end()) << "\n";
    return 0;
}
