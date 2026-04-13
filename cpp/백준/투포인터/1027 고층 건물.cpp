#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<int> building(N);
    for (int i = 0; i < N; ++i) cin >> building[i];

    int answer = 0;
    for (int i = 0; i < N; ++i) {
        int count = 0;
        double maxSlope = -1e18;
        for (int j = i + 1; j < N; ++j) {
            double slope = (double)(building[j] - building[i]) / (double)(j - i);
            if (slope > maxSlope) {
                maxSlope = slope;
                ++count;
            }
        }
        double minSlope = 1e18;
        for (int j = i - 1; j >= 0; --j) {
            double slope = (double)(building[j] - building[i]) / (double)(j - i);
            if (slope < minSlope) {
                minSlope = slope;
                ++count;
            }
        }
        answer = max(answer, count);
    }

    cout << answer << "\n";
    return 0;
}
