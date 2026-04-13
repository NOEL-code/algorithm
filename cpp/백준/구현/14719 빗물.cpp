#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int H, W;
    cin >> H >> W;

    vector<int> block(W);
    for (int i = 0; i < W; ++i) cin >> block[i];

    int water = 0;

    for (int i = 1; i < W - 1; ++i) {
        int left_max = *max_element(block.begin(), block.begin() + i);
        int right_max = *max_element(block.begin() + i + 1, block.end());

        int min_block = min(left_max, right_max);

        if (min_block - block[i] > 0) {
            water += min_block - block[i];
        }
    }

    cout << water << "\n";
    return 0;
}
