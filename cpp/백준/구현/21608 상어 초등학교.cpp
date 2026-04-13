#include <iostream>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <algorithm>
using namespace std;

int N;
unordered_map<int, vector<int>> like_dict;
vector<vector<int>> seat;

int directions[4][2] = {{0, 1}, {1, 0}, {-1, 0}, {0, -1}};

pair<int, int> find_best_seat(int student) {
    const vector<int>& likes = like_dict[student];
    bool has_best = false;
    tuple<int, int, int, int> best;

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            if (seat[r][c] != 0) continue;

            int like_count = 0;
            int empty_count = 0;

            for (int d = 0; d < 4; ++d) {
                int nr = r + directions[d][0];
                int nc = c + directions[d][1];
                if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                    int val = seat[nr][nc];
                    if (find(likes.begin(), likes.end(), val) != likes.end()) {
                        like_count++;
                    } else if (val == 0) {
                        empty_count++;
                    }
                }
            }

            tuple<int, int, int, int> cur = {like_count, empty_count, r, c};
            if (!has_best) {
                best = cur;
                has_best = true;
            } else {
                auto [lc, ec, br, bc] = best;
                if (like_count > lc) {
                    best = cur;
                } else if (like_count == lc) {
                    if (empty_count > ec) {
                        best = cur;
                    } else if (empty_count == ec) {
                        if (r < br || (r == br && c < bc)) {
                            best = cur;
                        }
                    }
                }
            }
        }
    }

    return {get<2>(best), get<3>(best)};
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);

    cin >> N;

    vector<int> order;
    for (int i = 0; i < N * N; ++i) {
        int student;
        cin >> student;
        vector<int> likes(4);
        for (int j = 0; j < 4; ++j) cin >> likes[j];
        like_dict[student] = likes;
        order.push_back(student);
    }

    seat.assign(N, vector<int>(N, 0));

    for (int student : order) {
        auto [r, c] = find_best_seat(student);
        seat[r][c] = student;
    }

    int score_table[] = {0, 1, 10, 100, 1000};
    int answer = 0;

    for (int r = 0; r < N; ++r) {
        for (int c = 0; c < N; ++c) {
            int student = seat[r][c];
            const vector<int>& likes = like_dict[student];
            int like_cnt = 0;

            for (int d = 0; d < 4; ++d) {
                int nr = r + directions[d][0];
                int nc = c + directions[d][1];
                if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                    if (find(likes.begin(), likes.end(), seat[nr][nc]) != likes.end()) {
                        like_cnt++;
                    }
                }
            }

            answer += score_table[like_cnt];
        }
    }

    cout << answer << "\n";
    return 0;
}
