#include <iostream>
#include <vector>
#include <algorithm>
#include <set>
using namespace std;

int N, M;
vector<int> num_arr;
vector<int> arr;

void back_tracking(int now_length, int start) {
    if (now_length == M) {
        for (int i = 0; i < M; ++i) {
            cout << arr[i];
            if (i + 1 < M) cout << ' ';
        }
        cout << '\n';
        return;
    }

    for (int idx = start; idx < (int)num_arr.size(); ++idx) {
        arr.push_back(num_arr[idx]);
        back_tracking(now_length + 1, idx);
        arr.pop_back();
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    vector<int> raw(N);
    for (int i = 0; i < N; ++i) cin >> raw[i];
    set<int> unique_set(raw.begin(), raw.end());
    num_arr.assign(unique_set.begin(), unique_set.end());

    back_tracking(0, 0);
    return 0;
}
