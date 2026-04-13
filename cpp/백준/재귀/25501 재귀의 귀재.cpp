#include <iostream>
#include <string>
#include <array>
using namespace std;

array<int, 2> recursion(const string& s, int l, int r, int cnt) {
    if (l >= r) return {1, cnt};
    if (s[l] != s[r]) return {0, cnt};
    return recursion(s, l + 1, r - 1, cnt + 1);
}

array<int, 2> isPalindrome(const string& s, int cnt) {
    return recursion(s, 0, (int)s.size() - 1, cnt);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        string s;
        cin >> s;
        auto result = isPalindrome(s, 1);
        cout << result[0] << ' ' << result[1] << "\n";
    }
    return 0;
}
