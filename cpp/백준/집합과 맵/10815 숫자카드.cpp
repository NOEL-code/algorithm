#include <iostream>
#include <unordered_set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;
    unordered_set<int> _dict;
    for (int i = 0; i < n; ++i) {
        int card;
        cin >> card;
        _dict.insert(card);
    }

    int m;
    cin >> m;
    for (int i = 0; i < m; ++i) {
        int check;
        cin >> check;
        if (_dict.find(check) == _dict.end()) cout << 0 << " ";
        else cout << 1 << " ";
    }
    cout << "\n";
    return 0;
}
