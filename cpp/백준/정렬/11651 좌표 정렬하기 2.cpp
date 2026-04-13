#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, int>> data(n);
    for (int i = 0; i < n; ++i) {
        int x, y;
        cin >> x >> y;
        data[i] = {y, x};
    }

    sort(data.begin(), data.end());

    for (int i = 0; i < n; ++i) {
        cout << data[i].second << ' ' << data[i].first << "\n";
    }
    return 0;
}
