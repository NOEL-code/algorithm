#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    vector<long long> d(101, 0);
    d[1] = 1;
    d[2] = 1;
    d[3] = 1;
    d[4] = 2;
    d[5] = 2;
    for (int i = 6; i <= 100; ++i) {
        d[i] = d[i - 5] + d[i - 1];
    }

    while (T--) {
        int N;
        cin >> N;
        cout << d[N] << "\n";
    }

    return 0;
}
