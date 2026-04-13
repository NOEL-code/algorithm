#include <iostream>
#include <vector>
#include <deque>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    const int MAX_POSITION = 100000;
    int N, K;
    cin >> N >> K;

    vector<int> visited(MAX_POSITION + 1, -1);
    deque<int> q;
    q.push_back(N);
    visited[N] = 0;

    while (!q.empty()) {
        int current = q.front();
        q.pop_front();

        if (current == K) {
            cout << visited[current] << "\n";
            return 0;
        }

        if (current * 2 >= 0 && current * 2 <= MAX_POSITION && visited[current * 2] == -1) {
            visited[current * 2] = visited[current];
            q.push_front(current * 2);
        }

        if (current - 1 >= 0 && visited[current - 1] == -1) {
            visited[current - 1] = visited[current] + 1;
            q.push_back(current - 1);
        }

        if (current + 1 <= MAX_POSITION && visited[current + 1] == -1) {
            visited[current + 1] = visited[current] + 1;
            q.push_back(current + 1);
        }
    }

    return 0;
}
