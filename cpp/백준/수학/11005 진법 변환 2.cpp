#include <iostream>
#include <string>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    long long n;
    int b;
    cin >> n >> b;

    string array = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    string result = "";

    while (n > 0) {
        result += array[n % b];
        n = n / b;
    }

    reverse(result.begin(), result.end());
    cout << result << "\n";
    return 0;
}
