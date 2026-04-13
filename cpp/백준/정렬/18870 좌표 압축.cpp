#include <iostream>
#include <vector>
#include <algorithm>
#include <unordered_map>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> array(n);
    for (int i = 0; i < n; ++i) cin >> array[i];

    vector<int> array_s(array);
    sort(array_s.begin(), array_s.end());
    array_s.erase(unique(array_s.begin(), array_s.end()), array_s.end());

    unordered_map<int, int> dic;
    for (int i = 0; i < (int)array_s.size(); ++i) {
        dic[array_s[i]] = i;
    }

    for (int a : array) {
        cout << dic[a] << ' ';
    }
    cout << "\n";
    return 0;
}
