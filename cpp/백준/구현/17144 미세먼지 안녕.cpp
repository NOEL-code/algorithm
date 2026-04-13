#include <iostream>
#include <vector>
using namespace std;

int R, C, T;

void diffuse(vector<vector<int>>& board, vector<vector<int>>& temp_board) {
    int dx[] = {1, 0, -1, 0};
    int dy[] = {0, 1, 0, -1};

    for (int r = 0; r < R; ++r) {
        for (int c = 0; c < C; ++c) {
            int cnt = 0;
            if (board[r][c] > 0) {
                for (int d = 0; d < 4; ++d) {
                    int nx = r + dx[d];
                    int ny = c + dy[d];
                    if (nx >= 0 && nx < R && ny >= 0 && ny < C && board[nx][ny] > -1) {
                        temp_board[nx][ny] += board[r][c] / 5;
                        cnt++;
                    }
                }
                temp_board[r][c] -= (board[r][c] / 5) * cnt;
            }
        }
    }

    for (int r = 0; r < R; ++r)
        for (int c = 0; c < C; ++c)
            board[r][c] += temp_board[r][c];
}

void air_cleaner(vector<vector<int>>& board, int top) {
    for (int i = top - 1; i > 0; --i)
        board[i][0] = board[i - 1][0];
    for (int i = 0; i < C - 1; ++i)
        board[0][i] = board[0][i + 1];
    for (int i = 0; i < top; ++i)
        board[i][C - 1] = board[i + 1][C - 1];
    for (int i = C - 1; i > 1; --i)
        board[top][i] = board[top][i - 1];
    board[top][1] = 0;

    int bot = top + 1;
    for (int i = bot + 1; i < R - 1; ++i)
        board[i][0] = board[i + 1][0];
    for (int i = 0; i < C - 1; ++i)
        board[R - 1][i] = board[R - 1][i + 1];
    for (int i = R - 1; i > bot; --i)
        board[i][C - 1] = board[i - 1][C - 1];
    for (int i = C - 1; i > 1; --i)
        board[bot][i] = board[bot][i - 1];
    board[bot][1] = 0;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> R >> C >> T;

    vector<vector<int>> board(R, vector<int>(C));
    for (int i = 0; i < R; ++i)
        for (int j = 0; j < C; ++j)
            cin >> board[i][j];

    int top = 0;
    for (int i = 0; i < R; ++i) {
        if (board[i][0] == -1) {
            top = i;
            break;
        }
    }

    for (int t = 0; t < T; ++t) {
        vector<vector<int>> temp_board(R, vector<int>(C, 0));
        diffuse(board, temp_board);
        air_cleaner(board, top);
    }

    int answer = 0;
    for (int i = 0; i < R; ++i)
        for (int j = 0; j < C; ++j)
            answer += board[i][j];

    cout << answer + 2 << "\n";
    return 0;
}
