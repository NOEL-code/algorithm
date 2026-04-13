#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

int R, C;
vector<string> B;
int answer = 0;
bool check[26];

int dr[] = {1, -1, 0, 0};
int dc[] = {0, 0, 1, -1};

void dfs(int r, int c, int count) {
    answer = max(answer, count);
    for (int i = 0; i < 4; ++i) {
        int nr = r + dr[i];
        int nc = c + dc[i];
        if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
        int idx = B[nr][nc] - 'A';
        if (check[idx]) continue;
        check[idx] = true;
        dfs(nr, nc, count + 1);
        check[idx] = false;
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> R >> C;
    B.assign(R, "");
    for (int i = 0; i < R; ++i) cin >> B[i];

    check[B[0][0] - 'A'] = true;
    dfs(0, 0, 1);
    cout << answer << "\n";
    return 0;
}
