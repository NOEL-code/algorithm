#include <iostream>
#include <string>
#include <stack>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string line;
    while (getline(cin, line)) {
        if (line == ".") break;

        stack<char> st;
        bool broken = false;

        for (char c : line) {
            if (c == '(' || c == '[') {
                st.push(c);
            } else if (c == ')') {
                if (!st.empty() && st.top() == '(') {
                    st.pop();
                } else {
                    broken = true;
                    break;
                }
            } else if (c == ']') {
                if (!st.empty() && st.top() == '[') {
                    st.pop();
                } else {
                    broken = true;
                    break;
                }
            }
        }

        if (!broken && st.empty()) cout << "yes\n";
        else cout << "no\n";
    }
    return 0;
}
