#include <iostream>
#include <queue>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    priority_queue<int> heap;

    while (n--) {
        int num;
        cin >> num;

        if (num != 0) {
            heap.push(num);
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
