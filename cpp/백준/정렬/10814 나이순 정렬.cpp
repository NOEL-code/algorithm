#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<pair<int, string>> data(n);
    for (int i = 0; i < n; ++i) {
        cin >> data[i].first >> data[i].second;
    }

    stable_sort(data.begin(), data.end(), [](const pair<int, string>& a, const pair<int, string>& b) {
        return a.first < b.first;
    });

    for (int i = 0; i < n; ++i) {
        cout << data[i].first << ' ' << data[i].second << "\n";
    }
    return 0;
}
