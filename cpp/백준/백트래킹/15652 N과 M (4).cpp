#include <iostream>
#include <vector>
using namespace std;

int N, M;
vector<int> arr;

void back_tracking(int now_length, int current_num) {
    if (now_length == M) {
        for (int i = 0; i < M; ++i) {
            cout << arr[i];
            if (i + 1 < M) cout << ' ';
        }
        cout << '\n';
        return;
    }

    for (int idx = current_num; idx <= N; ++idx) {
        arr.push_back(idx);
        back_tracking(now_length + 1, idx);
        arr.pop_back();
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    back_tracking(0, 1);
    return 0;
}
