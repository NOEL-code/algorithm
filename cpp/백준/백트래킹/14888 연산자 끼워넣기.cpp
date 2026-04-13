#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int N;
vector<int> nums;
int min_num = INT_MAX;
int max_num = INT_MIN;

void dfs(int idx, int total, int plus, int minus, int multiply, int divide) {
    if (idx == N) {
        max_num = max(total, max_num);
        min_num = min(total, min_num);
        return;
    }

    if (plus > 0)
        dfs(idx + 1, total + nums[idx], plus - 1, minus, multiply, divide);
    if (minus > 0)
        dfs(idx + 1, total - nums[idx], plus, minus - 1, multiply, divide);
    if (multiply > 0)
        dfs(idx + 1, total * nums[idx], plus, minus, multiply - 1, divide);
    if (divide > 0) {
        int q;
        if (total < 0) q = -((-total) / nums[idx]);
        else q = total / nums[idx];
        dfs(idx + 1, q, plus, minus, multiply, divide - 1);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;
    nums.resize(N);
    for (int i = 0; i < N; ++i) cin >> nums[i];

    vector<int> operand(4);
    for (int i = 0; i < 4; ++i) cin >> operand[i];

    dfs(1, nums[0], operand[0], operand[1], operand[2], operand[3]);
    cout << max_num << "\n";
    cout << min_num << "\n";
    return 0;
}
