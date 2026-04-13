#include <iostream>
using namespace std;

long long dac(long long a, long long b, long long c) {
    if (b == 1) return a % c;
    long long half = dac(a, b / 2, c);
    if (b % 2 == 0) {
        return (half * half) % c;
    } else {
        return ((half * half) % c * a) % c;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b, c;
    cin >> a >> b >> c;
    cout << dac(a, b, c) << "\n";
    return 0;
}
