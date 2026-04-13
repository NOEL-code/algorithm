#include <iostream>
#include <queue>
#include <cstdlib>
using namespace std;

struct Cmp {
    bool operator()(const int& a, const int& b) const {
        int aa = abs(a), bb = abs(b);
        if (aa != bb) return aa > bb;
        return a > b;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    priority_queue<int, vector<int>, Cmp> heap;

    while (n--) {
        int x;
        cin >> x;

        if (x != 0) {
            heap.push(x);
        } else {
            if (!heap.empty()) {
                cout << heap.top() << "\n";
                heap.pop();
            } else {
                cout << 0 << "\n";
            }
        }
    }
    return 0;
}
