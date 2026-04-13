#include <iostream>
#include <string>
#include <unordered_set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    int count = 0;
    unordered_set<string> _seen;

    for (int i = 0; i < N; ++i) {
        string nickname;
        cin >> nickname;
        if (nickname == "ENTER") {
            _seen.clear();
        } else if (_seen.find(nickname) == _seen.end()) {
            _seen.insert(nickname);
            count += 1;
        }
    }

    cout << count << "\n";
    return 0;
}
