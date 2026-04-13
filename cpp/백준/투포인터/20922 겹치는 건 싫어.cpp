#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;
    vector<int> arr(N);
    for (int i = 0; i < N; ++i) cin >> arr[i];

    vector<int> cnt(100001, 0);
    int left = 0, right = 0, answer = 0;

    while (right < N) {
        if (cnt[arr[right]] < K) {
            cnt[arr[right]] += 1;
            right += 1;
            answer = max(answer, right - left);
        } else {
            cnt[arr[left]] -= 1;
            left += 1;
        }
    }

    cout << answer << "\n";
    return 0;
}
