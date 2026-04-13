#include <iostream>
#include <string>
#include <unordered_set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m;
    cin >> n >> m;

    unordered_set<string> s;
    int count = 0;

    for (int i = 0; i < n; ++i) {
        string t;
        cin >> t;
        s.insert(t);
    }

    for (int i = 0; i < m; ++i) {
        string t;
        cin >> t;
        if (s.find(t) != s.end()) count += 1;
    }

    cout << count << "\n";
    return 0;
}
