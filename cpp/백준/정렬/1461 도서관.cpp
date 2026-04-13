#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <cmath>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    vector<int> left, right;
    for (int i = 0; i < N; ++i) {
        int x;
        cin >> x;
        if (x < 0) left.push_back(abs(x));
        else right.push_back(x);
    }

    sort(left.begin(), left.end(), greater<int>());
    sort(right.begin(), right.end(), greater<int>());

    long long result = 0;
    for (size_t i = 0; i < left.size(); i += M) {
        result += (long long)left[i] * 2;
    }
    for (size_t i = 0; i < right.size(); i += M) {
        result += (long long)right[i] * 2;
    }

    int max_dist = 0;
    if (!left.empty()) max_dist = max(max_dist, left[0]);
    if (!right.empty()) max_dist = max(max_dist, right[0]);

    result -= max_dist;

    cout << result << "\n";
    return 0;
}
