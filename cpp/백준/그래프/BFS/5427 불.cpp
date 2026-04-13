#include <iostream>
#include <vector>
#include <queue>
#include <string>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int dy[] = {1, -1, 0, 0};
    int dx[] = {0, 0, 1, -1};
    const int INF = 1000000000;

    int T;
    cin >> T;
    while (T--) {
        int w, h;
        cin >> w >> h;

        vector<vector<int>> person_dist(h, vector<int>(w, -1));
        vector<vector<int>> fire_dist(h, vector<int>(w, INF));
        vector<string> building(h);
        queue<pair<int, int>> fire_queue, person_queue;

        for (int y = 0; y < h; ++y) {
            cin >> building[y];
            for (int x = 0; x < w; ++x) {
                char ch = building[y][x];
                if (ch == '@') {
                    person_dist[y][x] = 0;
                    person_queue.push({y, x});
                    building[y][x] = '.';
                } else if (ch == '*') {
                    fire_dist[y][x] = 0;
                    fire_queue.push({y, x});
                }
            }
        }

        while (!fire_queue.empty()) {
            auto [y, x] = fire_queue.front();
            fire_queue.pop();
            for (int i = 0; i < 4; ++i) {
                int ny = y + dy[i];
                int nx = x + dx[i];
                if (ny < 0 || ny >= h || nx < 0 || nx >= w) continue;
                if (building[ny][nx] != '#' && fire_dist[ny][nx] == INF) {
                    fire_dist[ny][nx] = fire_dist[y][x] + 1;
                    fire_queue.push({ny, nx});
                }
            }
        }

        int escaped_time = -1;
        bool done = false;
        while (!person_queue.empty() && !done) {
            auto [y, x] = person_queue.front();
            person_queue.pop();
            int curr_time = person_dist[y][x];

            for (int i = 0; i < 4; ++i) {
                int ny = y + dy[i];
                int nx = x + dx[i];
                if (ny < 0 || ny >= h || nx < 0 || nx >= w) {
                    escaped_time = curr_time + 1;
                    done = true;
                    break;
                }
                if (building[ny][nx] != '#' && person_dist[ny][nx] == -1) {
                    int next_time = curr_time + 1;
                    if (next_time < fire_dist[ny][nx]) {
                        person_dist[ny][nx] = next_time;
                        person_queue.push({ny, nx});
                    }
                }
            }
        }

        if (escaped_time == -1) cout << "IMPOSSIBLE" << "\n";
        else cout << escaped_time << "\n";
    }
    return 0;
}
