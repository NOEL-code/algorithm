#include <iostream>
#include <string>
#include <set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    set<string> _dict;
    for (int i = 0; i < n; ++i) {
        string name, command;
        cin >> name >> command;
        if (command == "enter") {
            _dict.insert(name);
        } else {
            _dict.erase(name);
        }
    }

    for (auto it = _dict.rbegin(); it != _dict.rend(); ++it) {
        cout << *it << "\n";
    }
    return 0;
}
