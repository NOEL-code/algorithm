#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> d(n + 1, 0);
    d[1] = 1;
    if (n >= 2) d[2] = 3;
    for (int i = 3; i <= n; ++i) {
        d[i] = ((d[i - 2] * 2) + d[i - 1]) % 10007;
    }

    cout << d[n] % 10007 << "\n";
    return 0;
}
