#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> array(10000, 0);
    for (int i = 0; i < n; ++i) cin >> array[i];

    vector<int> d(10000, 0);

    d[0] = array[0];
    if (n >= 2) d[1] = array[0] + array[1];
    if (n >= 3) d[2] = max({array[2] + array[0], array[2] + array[1], d[1]});

    for (int i = 3; i < n; ++i) {
        d[i] = max({array[i] + d[i - 2], array[i] + array[i - 1] + d[i - 3], d[i - 1]});
    }

    cout << *max_element(d.begin(), d.end()) << "\n";
    return 0;
}
