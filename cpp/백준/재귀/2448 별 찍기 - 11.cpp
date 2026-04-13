#include <iostream>
#include <string>
using namespace std;

char star(int n, int r, int c) {
    if (n == 1) return '*';

    int block = n / 3;
    if ((r / block) == 1 && (c / block) == 1) {
        return ' ';
    }
    return star(block, r % block, c % block);
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    for (int r = 0; r < n; ++r) {
        string row;
        row.reserve(n);
        for (int c = 0; c < n; ++c) {
            row += star(n, r, c);
        }
        cout << row << "\n";
    }
    return 0;
}
