#include <iostream>
#include <vector>
#include <stack>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<int> tower(N);
    for (int i = 0; i < N; ++i) cin >> tower[i];

    stack<pair<int, int>> st;
    vector<int> answer;
    answer.reserve(N);

    for (int i = 0; i < N; ++i) {
        int t_idx = i + 1;
        int t_height = tower[i];

        while (!st.empty() && st.top().second < t_height) st.pop();

        if (!st.empty() && st.top().second >= t_height) {
            answer.push_back(st.top().first);
        } else {
            answer.push_back(0);
        }

        st.push({t_idx, t_height});
    }

    for (int i = 0; i < (int)answer.size(); ++i) cout << answer[i] << " ";
    cout << "\n";
    return 0;
}
