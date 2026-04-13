#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<int> sum(N + 1, 0);
    int temp = 0;
    for (int k = 1; k <= N; ++k) {
        int x;
        cin >> x;
        temp += x;
        sum[k] = temp;
    }

    for (int q = 0; q < M; ++q) {
        int i, j;
        cin >> i >> j;
        cout << sum[j] - sum[i - 1] << "\n";
    }

    return 0;
}
