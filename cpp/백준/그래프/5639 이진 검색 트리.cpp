#include <iostream>
#include <vector>
using namespace std;

vector<int> pre;

void post(int start, int end) {
    if (start > end) return;
    int mid = end + 1;
    for (int i = start + 1; i <= end; ++i) {
        if (pre[i] > pre[start]) {
            mid = i;
            break;
        }
    }
    post(start + 1, mid - 1);
    post(mid, end);
    cout << pre[start] << "\n";
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int x;
    while (cin >> x) pre.push_back(x);

    post(0, (int)pre.size() - 1);
    return 0;
}
