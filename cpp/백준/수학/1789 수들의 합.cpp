#include <iostream>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    long long s;
    cin >> s;

    long long i = 0;
    long long sum = 0;
    while (s > sum) {
        i += 1;
        sum += i;
    }

    if (sum == s) {
        cout << i << "\n";
    } else {
        cout << i - 1 << "\n";
    }
    return 0;
}
