#include <iostream>
#include <vector>
using namespace std;

int N, S;
vector<int> nums;
int count_ = 0;

void dfs(int idx, int sum, bool picked) {
    if (idx == N) {
        if (picked && sum == S) count_++;
        return;
    }
    dfs(idx + 1, sum, picked);
    dfs(idx + 1, sum + nums[idx], true);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> S;
    nums.resize(N);
    for (int i = 0; i < N; ++i) cin >> nums[i];

    dfs(0, 0, false);
    cout << count_ << "\n";
    return 0;
}
