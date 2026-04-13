#include <iostream>
#include <string>
using namespace std;

void hanoi(int n, int start, int end, int via) {
    if (n == 1) {
        cout << start << ' ' << end << "\n";
        return;
    }
    hanoi(n - 1, start, via, end);
    cout << start << ' ' << end << "\n";
    hanoi(n - 1, via, end, start);
}

string powerOfTwoMinusOne(int n) {
    string result = "1";
    for (int i = 0; i < n; ++i) {
        int carry = 0;
        for (int j = (int)result.size() - 1; j >= 0; --j) {
            int digit = (result[j] - '0') * 2 + carry;
            result[j] = '0' + digit % 10;
            carry = digit / 10;
        }
        while (carry > 0) {
            result = char('0' + carry % 10) + result;
            carry /= 10;
        }
    }
    int carry = -1;
    for (int j = (int)result.size() - 1; j >= 0 && carry != 0; --j) {
        int digit = (result[j] - '0') + carry;
        if (digit < 0) {
            result[j] = '9';
            carry = -1;
        } else {
            result[j] = '0' + digit;
            carry = 0;
        }
    }
    return result;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    cin >> n;

    cout << powerOfTwoMinusOne(n) << "\n";

    if (n <= 20) {
        hanoi(n, 1, 3, 2);
    }
    return 0;
}
