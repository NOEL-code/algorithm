#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    const int MAX_POSITION = 100000;
    int N, K;
    cin >> N >> K;

    vector<int> visited(MAX_POSITION + 1, -1);

    if (N == K) {
        cout << 0 << "\n";
        return 0;
    }

    queue<int> q;
    q.push(N);
    visited[N] = 0;

    while (!q.empty()) {
        int current = q.front();
        q.pop();

        int nexts[] = {current - 1, current + 1, current * 2};
        for (int next : nexts) {
            if (next >= 0 && next <= MAX_POSITION && visited[next] == -1) {
                visited[next] = visited[current] + 1;
                q.push(next);
                if (next == K) {
                    cout << visited[next] << "\n";
                    return 0;
                }
            }
        }
    }

    return 0;
}
