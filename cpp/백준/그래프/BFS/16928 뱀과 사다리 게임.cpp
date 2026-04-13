#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    unordered_map<int, int> ladder, snake;
    for (int i = 0; i < N; ++i) {
        int start, end;
        cin >> start >> end;
        ladder[start] = end;
    }
    for (int i = 0; i < M; ++i) {
        int start, end;
        cin >> start >> end;
        snake[start] = end;
    }

    vector<int> visited(101, -1);
    queue<int> q;
    q.push(1);
    visited[1] = 0;

    while (!q.empty()) {
        int node = q.front();
        q.pop();

        if (node == 100) {
            cout << visited[node] << "\n";
            return 0;
        }

        for (int dice_num = 1; dice_num <= 6; ++dice_num) {
            int next_node = node + dice_num;
            if (next_node > 100) continue;
            if (ladder.count(next_node)) next_node = ladder[next_node];
            else if (snake.count(next_node)) next_node = snake[next_node];

            if (visited[next_node] == -1) {
                visited[next_node] = visited[node] + 1;
                q.push(next_node);
            }
        }
    }

    cout << -1 << "\n";
    return 0;
}
