#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;

    vector<int> queue(N);
    for (int i = 0; i < N; ++i) cin >> queue[i];

    sort(queue.begin(), queue.end());

    int total = 0;
    for (int i = 0; i < N; ++i) {
        total += queue[i];
        if (i < N - 1) {
            queue[i + 1] += queue[i];
        }
    }

    cout << total << "\n";
    return 0;
}
