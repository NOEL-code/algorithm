#include <iostream>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    vector<int> fruits(N);
    for (int i = 0; i < N; ++i) cin >> fruits[i];

    int left = 0;
    unordered_map<int, int> fruit_count;
    int max_length = 0;

    for (int right = 0; right < N; ++right) {
        fruit_count[fruits[right]] += 1;
        while ((int)fruit_count.size() > 2) {
            fruit_count[fruits[left]] -= 1;
            if (fruit_count[fruits[left]] == 0) {
                fruit_count.erase(fruits[left]);
            }
            left += 1;
        }
        max_length = max(max_length, right - left + 1);
    }

    cout << max_length << "\n";
    return 0;
}
