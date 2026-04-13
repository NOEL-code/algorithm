#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int N;
        cin >> N;

        vector<int> coins(N);
        for (int i = 0; i < N; ++i) cin >> coins[i];

        int target;
        cin >> target;

        vector<int> d(target + 1, 0);
        d[0] = 1;

        for (int coin : coins) {
            for (int money = coin; money <= target; ++money) {
                d[money] += d[money - coin];
            }
        }

        cout << d[target] << "\n";
    }

    return 0;
}
