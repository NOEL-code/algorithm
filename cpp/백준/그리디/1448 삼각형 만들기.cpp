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

    vector<int> straw(n);
    for (int i = 0; i < n; ++i) cin >> straw[i];

    sort(straw.begin(), straw.end(), greater<int>());

    for (int i = 0; i < n - 2; ++i) {
        int a = straw[i], b = straw[i + 1], c = straw[i + 2];
        if (a < b + c) {
            cout << a + b + c << "\n";
            return 0;
        }
    }

    cout << -1 << "\n";
    return 0;
}
