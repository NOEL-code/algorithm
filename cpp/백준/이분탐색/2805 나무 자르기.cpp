#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    long long M;
    cin >> N >> M;
    vector<long long> trees(N);
    for (int i = 0; i < N; ++i) cin >> trees[i];

    long long start = 0;
    long long end = *max_element(trees.begin(), trees.end());
    long long result = 0;

    while (start <= end) {
        long long mid = (start + end) / 2;
        long long log = 0;
        for (long long i : trees) {
            if (i >= mid) log += i - mid;
        }
        if (log < M) {
            end = mid - 1;
        } else {
            result = mid;
            start = mid + 1;
        }
    }

    cout << result << "\n";
    return 0;
}
