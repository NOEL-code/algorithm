#include <iostream>
#include <vector>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M;
    cin >> N >> M;

    int r, c, d;
    cin >> r >> c >> d;

    vector<vector<int>> room(N, vector<int>(M));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            cin >> room[i][j];

    int dr[] = {-1, 0, 1, 0};
    int dc[] = {0, 1, 0, -1};

    int count = 0;
    while (true) {
        if (room[r][c] == 0) {
            room[r][c] = 2;
            ++count;
        }

        bool moved = false;
        for (int i = 0; i < 4; ++i) {
            d = (d + 3) % 4;
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < N && nc >= 0 && nc < M && room[nr][nc] == 0) {
                r = nr;
                c = nc;
                moved = true;
                break;
            }
        }

        if (moved) continue;

        int backDir = (d + 2) % 4;
        int nr = r + dr[backDir];
        int nc = c + dc[backDir];

        if (nr >= 0 && nr < N && nc >= 0 && nc < M && room[nr][nc] != 1) {
            r = nr;
            c = nc;
        } else {
            cout << count << "\n";
            break;
        }
    }

    return 0;
}
