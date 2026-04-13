#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<int> array(N);
    for (int i = 0; i < N; ++i) cin >> array[i];

    vector<int> d(N, 0);
    for (int i = 0; i < N; ++i) d[i] = array[i];

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < i; ++j) {
            if (array[i] > array[j]) {
                d[i] = max(d[i], array[i] + d[j]);
            }
        }
    }

    cout << *max_element(d.begin(), d.end()) << "\n";
    return 0;
}
