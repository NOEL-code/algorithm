#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    vector<int> array(9);
    for (int i = 0; i < 9; ++i) cin >> array[i];
    sort(array.begin(), array.end());

    int total = accumulate(array.begin(), array.end(), 0);

    for (int i = 0; i < 9; ++i) {
        for (int j = i + 1; j < 9; ++j) {
            if (total - (array[i] + array[j]) == 100) {
                for (int k = 0; k < 9; ++k) {
                    if (k == i || k == j) continue;
                    cout << array[k] << "\n";
                }
                return 0;
            }
        }
    }
    return 0;
}
