#include <iostream>
#include <vector>
using namespace std;

int N;
vector<vector<int>> paper;
int white_count = 0;
int blue_count = 0;

void divide(int x, int y, int size) {
    int color = paper[x][y];

    for (int i = x; i < x + size; ++i) {
        for (int j = y; j < y + size; ++j) {
            if (paper[i][j] != color) {
                int new_size = size / 2;
                divide(x, y, new_size);
                divide(x, y + new_size, new_size);
                divide(x + new_size, y, new_size);
                divide(x + new_size, y + new_size, new_size);
                return;
            }
        }
    }

    if (color == 0) white_count += 1;
    else blue_count += 1;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;
    paper.assign(N, vector<int>(N));
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            cin >> paper[i][j];

    divide(0, 0, N);

    cout << white_count << "\n" << blue_count << "\n";
    return 0;
}
