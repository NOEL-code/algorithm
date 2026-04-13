#include <iostream>
#include <string>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int M;
    cin >> M;

    int _set = 0;
    for (int i = 0; i < M; ++i) {
        string command;
        cin >> command;
        if (command == "all") {
            _set = (1 << 21) - 2;
        } else if (command == "empty") {
            _set = 0;
        } else {
            int x;
            cin >> x;
            int bit = 1 << x;
            if (command == "add") {
                _set |= bit;
            } else if (command == "remove") {
                _set &= ~bit;
            } else if (command == "check") {
                cout << ((_set & bit) ? 1 : 0) << "\n";
            } else if (command == "toggle") {
                _set ^= bit;
            }
        }
    }
    return 0;
}
