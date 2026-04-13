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
        int num;
        cin >> num;

        if (num == 0) {
            if (!st.empty()) st.pop();
        } else {
            st.push(num);
        }
    }

    long long sum = 0;
    while (!st.empty()) {
        sum += st.top();
        st.pop();
    }

    cout << sum << "\n";
    return 0;
}
