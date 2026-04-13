#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
using namespace std;

int N, M;
vector<pair<int, int>> chickens;
vector<pair<int, int>> homes;
int min_distance_all = INT_MAX;

int calculate_distance(const pair<int, int>& home, const vector<pair<int, int>>& selected) {
    int home_x = home.first, home_y = home.second;
    int min_distance = INT_MAX;
    for (auto& chicken : selected) {
        int distance = abs(home_x - chicken.first) + abs(home_y - chicken.second);
        min_distance = min(min_distance, distance);
    }
    return min_distance;
}

void combine(int start, vector<pair<int, int>>& selected) {
    if ((int)selected.size() == M) {
        int sum_distance = 0;
        for (auto& home : homes) {
            sum_distance += calculate_distance(home, selected);
        }
        min_distance_all = min(min_distance_all, sum_distance);
        return;
    }

    for (int i = start; i < (int)chickens.size(); ++i) {
        selected.push_back(chickens[i]);
        combine(i + 1, selected);
        selected.pop_back();
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N >> M;
    vector<vector<int>> board(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> board[i][j];
            if (board[i][j] == 2) {
                chickens.push_back({i, j});
            } else if (board[i][j] == 1) {
                homes.push_back({i, j});
            }
        }
    }

    vector<pair<int, int>> selected;
    combine(0, selected);

    cout << min_distance_all << "\n";
    return 0;
}
