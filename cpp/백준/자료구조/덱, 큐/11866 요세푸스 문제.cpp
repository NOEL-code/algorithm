#include <iostream>
#include <deque>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, k;
    cin >> n >> k;

    deque<int> circle;
    for (int i = 1; i <= n; ++i) circle.push_back(i);

    vector<int> result;
    while (!circle.empty()) {
        for (int i = 0; i < k - 1; ++i) {
            circle.push_back(circle.front());
            circle.pop_front();
        }
        result.push_back(circle.front());
        circle.pop_front();
    }

    cout << "<";
    for (int i = 0; i < (int)result.size(); ++i) {
        cout << result[i];
        if (i + 1 < (int)result.size()) cout << ", ";
    }
    cout << ">\n";
    return 0;
}
