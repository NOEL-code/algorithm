#include <iostream>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int sugar;
    cin >> sugar;

    int bag = 0;
    bool found = false;
    while (sugar >= 0) {
        if (sugar % 5 == 0) {
            bag += sugar / 5;
            cout << bag << "\n";
            found = true;
            break;
        }
        sugar -= 3;
        bag += 1;
    }

    if (!found) {
        cout << -1 << "\n";
    }

    return 0;
}
