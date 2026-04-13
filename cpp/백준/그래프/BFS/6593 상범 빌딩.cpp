#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <tuple>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int DZ[] = {1, -1, 0, 0, 0, 0};
    int DY[] = {0, 0, 1, -1, 0, 0};
    int DX[] = {0, 0, 0, 0, 1, -1};

    while (true) {
        int L, R, C;
        cin >> L >> R >> C;
        if (L == 0 && R == 0 && C == 0) break;

        vector<vector<string>> building(L, vector<string>(R));
        vector<vector<vector<int>>> dist(L, vector<vector<int>>(R, vector<int>(C, 0)));

        int start_z = -1, start_y = -1, start_x = -1;
        for (int z = 0; z < L; ++z) {
            for (int y = 0; y < R; ++y) {
                cin >> building[z][y];
                for (int x = 0; x < C; ++x) {
                    if (building[z][y][x] == 'S') {
                        start_z = z;
                        start_y = y;
                        start_x = x;
                    }
                }
            }
        }

        queue<tuple<int, int, int>> q;
        q.push({start_z, start_y, start_x});
        dist[start_z][start_y][start_x] = 1;

        bool escaped = false;
        int ans = 0;
        while (!q.empty() && !escaped) {
            auto [z, y, x] = q.front();
            q.pop();
            for (int i = 0; i < 6; ++i) {
                int nz = z + DZ[i];
                int ny = y + DY[i];
                int nx = x + DX[i];
                if (nz < 0 || nz >= L || ny < 0 || ny >= R || nx < 0 || nx >= C) continue;
                char cell = building[nz][ny][nx];
                if (cell == 'E') {
                    ans = dist[z][y][x];
                    escaped = true;
                    break;
                }
                if (cell == '.' && dist[nz][ny][nx] == 0) {
                    dist[nz][ny][nx] = dist[z][y][x] + 1;
                    q.push({nz, ny, nx});
                }
            }
        }

        if (escaped) cout << "Escaped in " << ans << " minute(s).\n";
        else cout << "Trapped!\n";
    }
    return 0;
}
