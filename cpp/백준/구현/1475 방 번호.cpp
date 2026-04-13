#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string nums;
    cin >> nums;

    vector<int> cnt(10, 0);
    for (char ch : nums) {
        cnt[ch - '0']++;
    }

    cnt[6] = cnt[9] = (cnt[6] + cnt[9] + 1) / 2;

    cout << *max_element(cnt.begin(), cnt.end()) << "\n";
    return 0;
}
