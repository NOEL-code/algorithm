#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int n;
vector<int> a;
vector<int> op;
int best_max = INT_MIN;
int best_min = INT_MAX;

void dfs(int idx, int cur) {
    if (idx == n) {
        best_max = max(best_max, cur);
        best_min = min(best_min, cur);
        return;
    }
    for (int k = 0; k < 4; ++k) {
        if (op[k] == 0) continue;
        op[k]--;
        int next;
        if (k == 0) next = cur + a[idx];
        else if (k == 1) next = cur - a[idx];
        else if (k == 2) next = cur * a[idx];
        else {
            if (cur < 0) next = -((-cur) / a[idx]);
            else next = cur / a[idx];
        }
        dfs(idx + 1, next);
        op[k]++;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> n;
    a.assign(n, 0);
    for (int i = 0; i < n; ++i) cin >> a[i];
    op.assign(4, 0);
    for (int i = 0; i < 4; ++i) cin >> op[i];

    dfs(1, a[0]);

    cout << best_max << "\n" << best_min << "\n";
    return 0;
}
