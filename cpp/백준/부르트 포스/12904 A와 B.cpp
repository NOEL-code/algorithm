#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string S, T;
    cin >> S >> T;

    while (T.size() > S.size()) {
        if (T.back() == 'A') {
            T.pop_back();
        } else {
            T.pop_back();
            reverse(T.begin(), T.end());
        }
    }

    cout << (T == S ? 1 : 0) << "\n";
    return 0;
}
