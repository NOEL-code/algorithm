#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int N, M;
vector<int> num_arr;
vector<bool> used_index_arr;
vector<int> arr;

void back_tracking(int now_length) {
    if (now_length == M) {
        for (int i = 0; i < M; ++i) {
            cout << arr[i];
            if (i + 1 < M) cout << ' ';
        }
        cout << '\n';
        return;
    }

    int prev = -1;
    for (int idx = 0; idx < (int)num_arr.size(); ++idx) {
        if (!used_index_arr[idx] && num_arr[idx] != prev) {
            used_index_arr[idx] = true;
            arr.push_back(num_arr[idx]);
            back_tracking(now_length + 1);
            used_index_arr[idx] = false;
            arr.pop_back();
            prev = num_arr[idx];
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    num_arr.resize(N);
    for (int i = 0; i < N; ++i) cin >> num_arr[i];
    sort(num_arr.begin(), num_arr.end());
    used_index_arr.assign(N, false);

    back_tracking(0);
    return 0;
}
