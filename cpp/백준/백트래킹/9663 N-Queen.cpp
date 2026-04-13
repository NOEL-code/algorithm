#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

int n;
int count_ = 0;
vector<int> row;

bool is_promising(int x) {
    for (int i = 0; i < x; ++i) {
        if (row[x] == row[i] || abs(row[x] - row[i]) == abs(x - i)) {
            return false;
        }
    }
    return true;
}

void n_queen(int x) {
    if (n == x) {
        count_ += 1;
        return;
    }

    for (int i = 0; i < n; ++i) {
        row[x] = i;
        if (is_promising(x)) {
            n_queen(x + 1);
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    row.assign(n, 0);
    n_queen(0);
    cout << count_ << "\n";
    return 0;
}
