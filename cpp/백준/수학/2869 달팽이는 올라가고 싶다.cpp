#include <iostream>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    long long a, b, v;
    cin >> a >> b >> v;

    long long days = (v - b) / (a - b);
    if ((v - b) % (a - b) != 0) {
        days += 1;
    }

    cout << days << "\n";
    return 0;
}
