#include <iostream>
#include <vector>
using namespace std;

int N, M;
vector<int> arr;
vector<bool> used;

void backTracking() {
    if ((int)arr.size() == M) {
        for (int i = 0; i < M; ++i) {
            cout << arr[i];
            if (i + 1 < M) cout << ' ';
        }
        cout << '\n';
        return;
    }

    for (int i = 1; i <= N; ++i) {
        if (!used[i]) {
            arr.push_back(i);
            used[i] = true;
            backTracking();
            arr.pop_back();
            used[i] = false;
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    used.assign(N + 1, false);
    backTracking();
    return 0;
}
