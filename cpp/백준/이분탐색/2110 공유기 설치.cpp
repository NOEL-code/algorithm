#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, C;
    cin >> N >> C;

    vector<int> data(N);
    for (int i = 0; i < N; ++i) cin >> data[i];
    sort(data.begin(), data.end());

    int start = 1;
    int end = data[N - 1] - data[0];
    int result = 0;

    while (start <= end) {
        int mid = (start + end) / 2;
        int count = 1;
        int last = data[0];
        for (int i = 1; i < N; ++i) {
            if (data[i] - last >= mid) {
                ++count;
                last = data[i];
            }
        }
        if (count >= C) {
            result = mid;
            start = mid + 1;
        } else {
            end = mid - 1;
        }
    }

    cout << result << "\n";
    return 0;
}
