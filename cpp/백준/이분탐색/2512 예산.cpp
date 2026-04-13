#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int N;
vector<int> budgets;
long long limit;

int search(int start, int end) {
    int result = 0;
    while (start <= end) {
        int mid = (start + end) / 2;
        long long total = 0;
        for (int num : budgets) total += min(mid, num);
        if (total <= limit) {
            result = mid;
            start = mid + 1;
        } else {
            end = mid - 1;
        }
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;
    budgets.resize(N);
    for (int i = 0; i < N; ++i) cin >> budgets[i];
    cin >> limit;

    int max_val = *max_element(budgets.begin(), budgets.end());
    long long sum = 0;
    for (int x : budgets) sum += x;

    if (sum <= limit) {
        cout << max_val << "\n";
    } else {
        cout << search(0, max_val) << "\n";
    }
    return 0;
}
