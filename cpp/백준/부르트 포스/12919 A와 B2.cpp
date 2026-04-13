#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

string S;
bool found = false;

void solve(string t) {
    if (found) return;
    if (t.size() == S.size()) {
        if (t == S) found = true;
        return;
    }

    if (t.back() == 'A') {
        solve(t.substr(0, t.size() - 1));
    }

    if (t.front() == 'B') {
        string rest = t.substr(1);
        reverse(rest.begin(), rest.end());
        solve(rest);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string T;
    cin >> S >> T;

    solve(T);
    cout << (found ? 1 : 0) << "\n";
    return 0;
}
