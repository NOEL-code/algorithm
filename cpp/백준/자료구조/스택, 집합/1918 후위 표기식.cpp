#include <iostream>
#include <string>
#include <stack>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string s;
    cin >> s;

    string result;
    stack<char> st;

    for (char c : s) {
        if (c == '+' || c == '-') {
            while (!st.empty() && st.top() != '(' &&
                   (st.top() == '+' || st.top() == '-' || st.top() == '*' || st.top() == '/')) {
                result += st.top();
                st.pop();
            }
            st.push(c);
        } else if (c == '*' || c == '/') {
            while (!st.empty() && st.top() != '(' &&
                   (st.top() == '*' || st.top() == '/')) {
                result += st.top();
                st.pop();
            }
            st.push(c);
        } else if (c == '(') {
            st.push('(');
        } else if (c == ')') {
            while (!st.empty() && st.top() != '(') {
                result += st.top();
                st.pop();
            }
            st.pop();
        } else {
            result += c;
        }
    }

    while (!st.empty()) {
        result += st.top();
        st.pop();
    }

    cout << result << "\n";
    return 0;
}
