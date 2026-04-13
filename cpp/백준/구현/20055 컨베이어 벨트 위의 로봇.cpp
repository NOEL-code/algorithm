#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, K;
    cin >> N >> K;

    deque<int> belt(2 * N);
    for (int i = 0; i < 2 * N; ++i) cin >> belt[i];

    deque<bool> robots(N, false);

    int step = 0;

    while (true) {
        step++;

        int last = belt.back();
        belt.pop_back();
        belt.push_front(last);

        robots.pop_back();
        robots.push_front(false);
        robots[N - 1] = false;

        for (int i = N - 2; i >= 0; --i) {
            if (robots[i] && !robots[i + 1] && belt[i + 1] > 0) {
                robots[i] = false;
                robots[i + 1] = true;
                belt[i + 1] -= 1;
            }
        }

        robots[N - 1] = false;

        if (belt[0] > 0) {
            robots[0] = true;
            belt[0] -= 1;
        }

        int zero_cnt = 0;
        for (int v : belt) if (v == 0) zero_cnt++;
        if (zero_cnt >= K) break;
    }

    cout << step << "\n";
    return 0;
}
