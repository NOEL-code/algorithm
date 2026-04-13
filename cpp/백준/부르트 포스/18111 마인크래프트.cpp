#include <iostream>
#include <vector>
#include <climits>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, M, B;
    cin >> N >> M >> B;

    vector<vector<int>> graph(N, vector<int>(M));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            cin >> graph[i][j];

    long long time = LLONG_MAX;
    int glevel = 0;

    for (int height = 0; height <= 256; ++height) {
        long long taken_block = 0;
        long long used_block = 0;

        for (int x = 0; x < N; ++x) {
            for (int y = 0; y < M; ++y) {
                if (graph[x][y] > height) {
                    taken_block += graph[x][y] - height;
                } else {
                    used_block += height - graph[x][y];
                }
            }
        }

        if (used_block > taken_block + B) continue;

        long long temp_time = (taken_block * 2) + used_block;

        if (temp_time <= time) {
            time = temp_time;
            glevel = height;
        }
    }

    cout << time << " " << glevel << "\n";
    return 0;
}
