#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int N;
vector<long long> nums;

bool search(int start, int end, long long target) {
    while (start <= end) {
        int mid = (start + end) / 2;
        if (nums[mid] == target) return true;
        else if (nums[mid] > target) end = mid - 1;
        else start = mid + 1;
    }
    return false;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;
    nums.resize(N);
    for (int i = 0; i < N; ++i) cin >> nums[i];
    sort(nums.begin(), nums.end());

    int count = 0;
    for (int i = 0; i < N - 2; ++i) {
        for (int j = i + 1; j < N - 1; ++j) {
            if (search(j + 1, N - 1, nums[i] + nums[j])) {
                count += 1;
            }
        }
    }

    cout << count << "\n";
    return 0;
}
