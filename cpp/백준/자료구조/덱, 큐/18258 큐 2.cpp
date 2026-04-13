#include <iostream>
#include <deque>
#include <string>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    deque<int> queue;

    while (n--) {
        string command;
        cin >> command;

        if (command == "push") {
            int x;
            cin >> x;
            queue.push_back(x);
        } else if (command == "pop") {
            if (!queue.empty()) {
                cout << queue.front() << "\n";
                queue.pop_front();
            } else {
                cout << -1 << "\n";
            }
        } else if (command == "size") {
            cout << queue.size() << "\n";
        } else if (command == "empty") {
            cout << (queue.empty() ? 1 : 0) << "\n";
        } else if (command == "front") {
            if (!queue.empty()) cout << queue.front() << "\n";
            else cout << -1 << "\n";
        } else if (command == "back") {
            if (!queue.empty()) cout << queue.back() << "\n";
            else cout << -1 << "\n";
        }
    }
    return 0;
}
