#include <iostream>
#include <string>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    string n;
    int b;
    cin >> n >> b;

    string array = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    long long result = 0;
    long long power = 1;
    for (int i = (int)n.size() - 1; i >= 0; --i) {
        result += power * (long long)array.find(n[i]);
        power *= b;
    }

    cout << result << "\n";
    return 0;
}
