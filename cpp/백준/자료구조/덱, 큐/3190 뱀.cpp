#include <iostream>
#include <deque>
#include <vector>
#include <set>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N;
    cin >> N;
    int K;
    cin >> K;

    set<pair<int, int>> apples;
    for (int i = 0; i < K; ++i) {
        int x, y;
        cin >> x >> y;
        apples.insert({x, y});
    }

    int L;
    cin >> L;

    deque<pair<int, char>> commands;
    for (int i = 0; i < L; ++i) {
        int second;
        char direction;
        cin >> second >> direction;
        commands.push_back({second, direction});
    }

    int answer = 0;
    int dx[] = {0, 1, 0, -1};
    int dy[] = {1, 0, -1, 0};

    deque<pair<int, int>> snake;
    snake.push_back({1, 1});
    set<pair<int, int>> snake_set;
    snake_set.insert({1, 1});
    int current_direction = 0;

    while (true) {
        answer += 1;

        auto [current_head_x, current_head_y] = snake.back();
        int next_head_x = current_head_x + dx[current_direction];
        int next_head_y = current_head_y + dy[current_direction];

        if (snake_set.count({next_head_x, next_head_y})) break;

        if (next_head_x > 0 && next_head_x <= N && next_head_y > 0 && next_head_y <= N) {
            snake.push_back({next_head_x, next_head_y});
            snake_set.insert({next_head_x, next_head_y});
        } else {
            break;
        }

        if (apples.count({next_head_x, next_head_y})) {
            apples.erase({next_head_x, next_head_y});
        } else {
            snake_set.erase(snake.front());
            snake.pop_front();
        }

        if (!commands.empty() && commands.front().first == answer) {
            char nd = commands.front().second;
            if (nd == 'D') current_direction = (current_direction + 1) % 4;
            else if (nd == 'L') current_direction = (current_direction - 1 + 4) % 4;
            commands.pop_front();
        }
    }

    cout << answer << "\n";
    return 0;
}
