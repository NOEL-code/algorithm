#include <iostream>
#include <deque>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    deque<pair<int, int>> dq;
    for (int i = 0; i < n; ++i) {
        int paper;
        cin >> paper;
        dq.push_back({i, paper});
    }

    while (!dq.empty()) {
        auto [index, paper] = dq.front();
        dq.pop_front();

        cout << index + 1 << " ";
        if (dq.empty()) break;

        if (paper > 0) {
            int r = (paper - 1) % (int)dq.size();
            for (int i = 0; i < r; ++i) {
                dq.push_back(dq.front());
                dq.pop_front();
            }
        } else if (paper < 0) {
            int r = (-paper) % (int)dq.size();
            for (int i = 0; i < r; ++i) {
                dq.push_front(dq.back());
                dq.pop_back();
            }
        }
    }
    cout << "\n";
    return 0;
}
