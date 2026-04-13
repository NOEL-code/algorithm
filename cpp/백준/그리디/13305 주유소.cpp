#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<long long> distances(N - 1);
    for (int i = 0; i < N - 1; ++i) cin >> distances[i];

    vector<long long> gas_prices(N);
    for (int i = 0; i < N; ++i) cin >> gas_prices[i];

    long long sum_price = 0;
    long long min_price = gas_prices[0];

    for (int i = 0; i < (int)distances.size(); ++i) {
        if (min_price > gas_prices[i]) {
            min_price = gas_prices[i];
        }
        sum_price += min_price * distances[i];
    }

    cout << sum_price << "\n";
    return 0;
}
