#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> card_cost(n + 1, 0);
    for (int i = 1; i <= n; ++i) cin >> card_cost[i];

    vector<int> d(n + 1, 0);

    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= i; ++j) {
            d[i] = max(d[i], d[i - j] + card_cost[j]);
        }
    }

    cout << d[n] << "\n";
    return 0;
}
