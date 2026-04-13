#include <iostream>
#include <vector>
#include <queue>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int F, S, G, U, D;
    cin >> F >> S >> G >> U >> D;

    int MAX_POSITION = F;
    vector<int> visited(MAX_POSITION + 1, -1);
    visited[S] = 0;

    if (S == G) {
        cout << 0 << "\n";
        return 0;
    }

    queue<int> q;
    q.push(S);

    while (!q.empty()) {
        int current = q.front();
        q.pop();
        if (current == G) break;

        int next_U = current + U;
        int next_D = current - D;

        if (next_U > 0 && next_U <= MAX_POSITION && visited[next_U] == -1) {
            visited[next_U] = visited[current] + 1;
            q.push(next_U);
        }

        if (next_D > 0 && next_D <= MAX_POSITION && visited[next_D] == -1) {
            visited[next_D] = visited[current] + 1;
            q.push(next_D);
        }
    }

    if (visited[G] == -1) cout << "use the stairs" << "\n";
    else cout << visited[G] << "\n";
    return 0;
}
