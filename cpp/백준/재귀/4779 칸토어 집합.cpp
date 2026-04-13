#include <iostream>
#include <string>
using namespace std;

string s;

void cantor_set(int start, int end_) {
    if (end_ == 1) return;

    int third = end_ / 3;
    for (int i = start + third; i < start + 2 * third; ++i) {
        s[i] = ' ';
    }

    cantor_set(start, third);
    cantor_set(start + 2 * third, third);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    while (cin >> N) {
        int len = 1;
        for (int i = 0; i < N; ++i) len *= 3;
        s.assign(len, '-');
        cantor_set(0, len);
        cout << s << "\n";
    }
    return 0;
}
