#include <iostream>
#include <stack>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    stack<int> st;

    while (n--) {
        int command;
        cin >> command;

        if (command == 1) {
            int x;
            cin >> x;
            st.push(x);
        } else if (command == 2) {
            if (!st.empty()) {
                cout << st.top() << "\n";
                st.pop();
            } else {
                cout << -1 << "\n";
            }
        } else if (command == 3) {
            cout << st.size() << "\n";
        } else if (command == 4) {
            cout << (st.empty() ? 1 : 0) << "\n";
        } else if (command == 5) {
            if (!st.empty()) cout << st.top() << "\n";
            else cout << -1 << "\n";
        }
    }
    return 0;
}
