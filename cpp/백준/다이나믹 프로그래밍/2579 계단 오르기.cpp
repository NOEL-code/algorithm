#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> steps(n + 1, 0);
    for (int i = 1; i <= n; ++i) cin >> steps[i];

    vector<int> d(n + 1, 0);

    if (n == 1) {
        cout << steps[1] << "\n";
        return 0;
    } else if (n == 2) {
        cout << steps[1] + steps[2] << "\n";
        return 0;
    }

    d[1] = steps[1];
    d[2] = steps[1] + steps[2];
    d[3] = max(steps[3] + steps[2], steps[3] + steps[1]);

    for (int i = 4; i <= n; ++i) {
        d[i] = max(d[i - 3] + steps[i - 1] + steps[i], d[i - 2] + steps[i]);
    }

    cout << d[n] << "\n";
    return 0;
}
