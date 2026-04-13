#include <iostream>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int num;
    cin >> num;
    int line = 1;

    while (num > line) {
        num -= line;
        line += 1;
    }

    int a, b;
    if (line % 2 == 0) {
        a = num;
        b = line - num + 1;
    } else {
        a = line - num + 1;
        b = num;
    }

    cout << a << '/' << b << "\n";
    return 0;
}
