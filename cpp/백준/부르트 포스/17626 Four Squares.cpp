#include <iostream>
#include <cmath>
using namespace std;

int n;

void solution(int n) {
    int r = (int)sqrt((double)n);
    if (r * r == n) {
        cout << 1 << "\n";
        exit(0);
    }

    for (int i = 1; i <= (int)sqrt((double)n); ++i) {
        int rem = n - i * i;
        int s = (int)sqrt((double)rem);
        if (s * s == rem) {
            cout << 2 << "\n";
            exit(0);
        }
    }

    for (int i = 1; i <= (int)sqrt((double)n); ++i) {
        for (int j = 1; j <= (int)sqrt((double)(n - i * i)); ++j) {
            int rem = n - i * i - j * j;
            int s = (int)sqrt((double)rem);
            if (s * s == rem) {
                cout << 3 << "\n";
                exit(0);
            }
        }
    }

    cout << 4 << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    solution(n);
    return 0;
}
