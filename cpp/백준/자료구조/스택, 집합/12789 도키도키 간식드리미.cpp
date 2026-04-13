#include <iostream>
#include <stack>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    vector<int> student(n);
    for (int i = 0; i < n; ++i) cin >> student[i];

    stack<int> st;
    int count = 1;

    for (int i = 0; i < n; ++i) {
        if (student[i] == count) {
            ++count;
        } else {
            st.push(student[i]);
        }

        while (!st.empty() && st.top() == count) {
            st.pop();
            ++count;
        }
    }

    cout << (st.empty() ? "Nice" : "Sad") << "\n";
    return 0;
}
