#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<long long> nums(N);
    for (int i = 0; i < N; ++i) cin >> nums[i];
    sort(nums.begin(), nums.end());

    long long best1 = nums[0], best2 = nums[1];
    long long bestAbs = llabs(nums[0] + nums[1]);

    int left = 0, right = N - 1;
    bool found = false;
    while (left < right) {
        long long s = nums[left] + nums[right];
        if (llabs(s) < bestAbs) {
            best1 = nums[left];
            best2 = nums[right];
            bestAbs = llabs(s);
        }
        if (s == 0) {
            cout << nums[left] << " " << nums[right] << "\n";
            found = true;
            break;
        }
        if (s > 0) --right;
        else ++left;
    }
    if (!found) {
        cout << best1 << " " << best2 << "\n";
    }
    return 0;
}
