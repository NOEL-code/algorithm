#include <iostream>
#include <vector>
#include <stack>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> array(n);
    for (int i = 0; i < n; ++i) cin >> array[i];

    vector<int> result(n, -1);
    stack<int> st;
    st.push(0);

    for (int i = 1; i < n; ++i) {
        while (!st.empty() && array[st.top()] < array[i]) {
            result[st.top()] = array[i];
            st.pop();
        }
        st.push(i);
    }

    for (int i = 0; i < n; ++i) {
        cout << result[i];
        if (i + 1 < n) cout << ' ';
    }
    cout << "\n";
    return 0;
}
