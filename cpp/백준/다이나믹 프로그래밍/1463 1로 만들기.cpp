#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> d(n + 1, 0);

    for (int i = 2; i <= n; ++i) {
        d[i] = d[i - 1] + 1;
        if (i % 3 == 0) {
            d[i] = min(d[i / 3] + 1, d[i]);
        }
        if (i % 2 == 0) {
            d[i] = min(d[i / 2] + 1, d[i]);
        }
    }

    cout << d[n] << "\n";
    return 0;
}
