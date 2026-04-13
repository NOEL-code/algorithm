#include <iostream>
#include <vector>
#include <array>
using namespace std;

array<int, 6> roll(const array<int, 6>& dice, int direction) {
    int top = dice[0], bottom = dice[1], north = dice[2], south = dice[3], west = dice[4], east = dice[5];
    if (direction == 1) return {west, east, north, south, bottom, top};
    if (direction == 2) return {east, west, north, south, top, bottom};
    if (direction == 3) return {south, north, top, bottom, west, east};
    return {north, south, bottom, top, west, east};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, x, y, K;
    cin >> N >> M >> x >> y >> K;

    vector<vector<int>> board(N, vector<int>(M));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            cin >> board[i][j];

    vector<int> commands(K);
    for (int i = 0; i < K; ++i) cin >> commands[i];

    array<int, 6> dice = {0, 0, 0, 0, 0, 0};

    int dx[] = {0, 0, 0, -1, 1};
    int dy[] = {0, 1, -1, 0, 0};

    for (int cmd : commands) {
        int nx = x + dx[cmd];
        int ny = y + dy[cmd];

        if (!(nx >= 0 && nx < N && ny >= 0 && ny < M)) continue;

        x = nx;
        y = ny;

        dice = roll(dice, cmd);

        if (board[x][y] == 0) {
            board[x][y] = dice[1];
        } else {
            dice[1] = board[x][y];
            board[x][y] = 0;
        }

        cout << dice[0] << "\n";
    }

    return 0;
}
