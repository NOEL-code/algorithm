#include <iostream>
using namespace std;

long long X, Y, Z;

long long search() {
    long long start = 1;
    long long end = (long long)1e9;
    long long min_value = (long long)1e9;

    while (start <= end) {
        long long mid = (start + end) / 2;
        long long percent = (long long)((double)(Y + mid) / (double)(X + mid) * 100);
        if (percent > Z) {
            end = mid - 1;
            min_value = mid;
        } else {
            start = mid + 1;
        }
    }
    return min_value;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> X >> Y;
    Z = (long long)((double)Y / (double)X * 100);

    if (Z >= 99) {
        cout << -1 << "\n";
    } else {
        cout << search() << "\n";
    }
    return 0;
}
