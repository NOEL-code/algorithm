#include <iostream>
using namespace std;

int N, r, c;
long long count_ = 0;

void searchZ(int x, int y, long long size) {
    if (size == 1) {
        if (x == r && y == c) {
            cout << count_ << "\n";
            exit(0);
        }
        count_ += 1;
        return;
    }

    long long new_size = size / 2;

    if (r < x + new_size && c < y + new_size) {
        searchZ(x, y, new_size);
    } else if (r < x + new_size && c >= y + new_size) {
        count_ += new_size * new_size;
        searchZ(x, y + new_size, new_size);
    } else if (r >= x + new_size && c < y + new_size) {
        count_ += 2 * new_size * new_size;
        searchZ(x + new_size, y, new_size);
    } else {
        count_ += 3 * new_size * new_size;
        searchZ(x + new_size, y + new_size, new_size);
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> r >> c;
    searchZ(0, 0, 1LL << N);
    return 0;
}
