#include <iostream>
#include <deque>
#include <string>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    deque<int> d;
    int n;
    cin >> n;

    while (n--) {
        int command;
        cin >> command;

        if (command == 1) {
            int x;
            cin >> x;
            d.push_front(x);
        } else if (command == 2) {
            int x;
            cin >> x;
            d.push_back(x);
        } else if (command == 3) {
            if (!d.empty()) {
                cout << d.front() << "\n";
                d.pop_front();
            } else {
                cout << -1 << "\n";
            }
        } else if (command == 4) {
            if (!d.empty()) {
                cout << d.back() << "\n";
                d.pop_back();
            } else {
                cout << -1 << "\n";
            }
        } else if (command == 5) {
            cout << d.size() << "\n";
        } else if (command == 6) {
            cout << (d.empty() ? 1 : 0) << "\n";
        } else if (command == 7) {
            if (!d.empty()) cout << d.front() << "\n";
            else cout << -1 << "\n";
        } else if (command == 8) {
            if (!d.empty()) cout << d.back() << "\n";
            else cout << -1 << "\n";
        }
    }
    return 0;
}
