#include <iostream>
#include <string>
#include <stack>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    while (n--) {
        string s;
        cin >> s;
        stack<char> st;
        bool flag = true;

        for (char c : s) {
            if (c == '(') {
                st.push('(');
            } else if (c == ')') {
                if (st.empty() || st.top() != '(') {
                    flag = false;
                    break;
                } else {
                    st.pop();
                }
            }
        }

        if (flag && st.empty()) cout << "YES\n";
        else cout << "NO\n";
    }
    return 0;
}
