#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> array_a(n), array_b(n);
    for (int i = 0; i < n; ++i) cin >> array_a[i];
    for (int i = 0; i < n; ++i) cin >> array_b[i];

    sort(array_a.begin(), array_a.end());
    sort(array_b.begin(), array_b.end(), greater<int>());

    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += (long long)array_b[i] * array_a[i];
    }

    cout << sum << "\n";
    return 0;
}
