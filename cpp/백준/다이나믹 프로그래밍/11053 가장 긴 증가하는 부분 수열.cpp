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

    vector<int> d(N, 1);

    for (int i = 1; i < N; ++i) {
        for (int j = 0; j < i; ++j) {
            if (array[i] > array[j]) {
                d[i] = max(d[i], d[j] + 1);
            }
        }
    }

    cout << *max_element(d.begin(), d.end()) << "\n";
    return 0;
}
