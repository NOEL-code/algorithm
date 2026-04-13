#include <iostream>
#include <string>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;
    string S;
    cin >> S;

    int count = 0;
    int pattern_length = 0;
    int i = 0;

    while (i < M - 2) {
        if (S.substr(i, 3) == "IOI") {
            pattern_length += 1;
            if (pattern_length == N) {
                count += 1;
                pattern_length -= 1;
            }
            i += 2;
        } else {
            pattern_length = 0;
            i += 1;
        }
    }

    cout << count << "\n";
    return 0;
}
