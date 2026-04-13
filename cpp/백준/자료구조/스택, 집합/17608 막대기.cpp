#include <iostream>
#include <stack>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    stack<int> st;
    int first;
    cin >> first;
    st.push(first);

    for (int i = 0; i < N - 1; ++i) {
        int num;
        cin >> num;
        while (!st.empty() && st.top() <= num) st.pop();
        st.push(num);
    }

    cout << st.size() << "\n";
    return 0;
}
