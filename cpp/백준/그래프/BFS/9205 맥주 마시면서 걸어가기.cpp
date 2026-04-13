#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    int T;
    cin >> T;
    while (T--) {
        int n;
        cin >> n;

        pair<int, int> home;
        cin >> home.first >> home.second;

        vector<pair<int, int>> conv(n);
        for (int i = 0; i < n; ++i) cin >> conv[i].first >> conv[i].second;

        pair<int, int> festival;
        cin >> festival.first >> festival.second;

        queue<pair<int, int>> q;
        q.push(home);
        vector<bool> visited(n, false);
        bool is_happy = false;

        while (!q.empty()) {
            auto [x, y] = q.front();
            q.pop();

            if (abs(x - festival.first) + abs(y - festival.second) <= 1000) {
                is_happy = true;
                break;
            }

            for (int i = 0; i < n; ++i) {
                if (!visited[i]) {
                    int nx = conv[i].first;
                    int ny = conv[i].second;
                    if (abs(x - nx) + abs(y - ny) <= 1000) {
                        visited[i] = true;
                        q.push({nx, ny});
                    }
                }
            }
        }

        cout << (is_happy ? "happy" : "sad") << "\n";
    }
    return 0;
}
