#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> array(n);
    for (int i = 0; i < n; ++i) cin >> array[i];

    sort(array.begin(), array.end());

    int maxVal = 0;
    for (int i = 0; i < n; ++i) {
        if (maxVal <= array[i] * (n - i)) {
            maxVal = array[i] * (n - i);
        }
    }

    cout << maxVal << "\n";
    return 0;
}
