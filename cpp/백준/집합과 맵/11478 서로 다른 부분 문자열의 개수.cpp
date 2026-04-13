#include <iostream>
#include <string>
#include <unordered_set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    unordered_set<string> count;
    int n = (int)s.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            count.insert(s.substr(i, j - i + 1));
        }
    }

    cout << count.size() << "\n";
    return 0;
}
