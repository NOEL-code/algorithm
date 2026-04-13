#include <iostream>
#include <set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;

    while (T--) {
        int Q;
        cin >> Q;

        multiset<int> ms;

        while (Q--) {
            char command;
            int num;
            cin >> command >> num;

            if (command == 'I') {
                ms.insert(num);
            } else {
                if (ms.empty()) continue;
                if (num == -1) {
                    ms.erase(ms.begin());
                } else {
                    auto it = ms.end();
                    --it;
                    ms.erase(it);
                }
            }
        }

        if (ms.empty()) {
            cout << "EMPTY\n";
        } else {
            auto last = ms.end();
            --last;
            cout << *last << " " << *ms.begin() << "\n";
        }
    }
    return 0;
}
