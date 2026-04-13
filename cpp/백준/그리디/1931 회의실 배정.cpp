#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<pair<int, int>> data(N);
    for (int i = 0; i < N; ++i) {
        int x, y;
        cin >> x >> y;
        data[i] = {x, y};
    }

    sort(data.begin(), data.end(), [](const pair<int, int>& a, const pair<int, int>& b) {
        if (a.second != b.second) return a.second < b.second;
        return a.first < b.first;
    });

    int count = 1;
    int end = data[0].second;

    for (int i = 1; i < N; ++i) {
        if (data[i].first >= end) {
            end = data[i].second;
            count += 1;
        }
    }

    cout << count << "\n";
    return 0;
}
