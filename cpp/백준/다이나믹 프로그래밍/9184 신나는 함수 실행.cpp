#include <iostream>
using namespace std;

int memo[51][51][51];
bool seen[51][51][51];

int w(int a, int b, int c) {
    if (a <= 0 || b <= 0 || c <= 0) return 1;
    if (a > 20 || b > 20 || c > 20) return w(20, 20, 20);

    if (seen[a][b][c]) return memo[a][b][c];

    int result;
    if (a < b && b < c) {
        result = w(a, b, c - 1) + w(a, b - 1, c - 1) - w(a, b - 1, c);
    } else {
        result = w(a - 1, b, c)
               + w(a - 1, b - 1, c)
               + w(a - 1, b, c - 1)
               - w(a - 1, b - 1, c - 1);
    }

    memo[a][b][c] = result;
    seen[a][b][c] = true;
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    while (true) {
        int a, b, c;
        cin >> a >> b >> c;
        if (a == -1 && b == -1 && c == -1) break;
        cout << "w(" << a << ", " << b << ", " << c << ") = " << w(a, b, c) << "\n";
    }

    return 0;
}
