#include <iostream>
#include <deque>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    deque<int> q;
    for (int i = 1; i <= n; ++i) q.push_back(i);

    vector<int> result;
    while (!q.empty()) {
        for (int i = 0; i < k - 1; ++i) {
            q.push_back(q.front());
            q.pop_front();
        }
        result.push_back(q.front());
        q.pop_front();
    }

    cout << "<";
    for (int i = 0; i < (int)result.size(); ++i) {
        cout << result[i];
        if (i + 1 < (int)result.size()) cout << ", ";
    }
    cout << ">\n";
    return 0;
}
